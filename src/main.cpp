#include <Arduino.h>
#include "config.h"
#include "secrets.h"
#include "WiFiModule.h"
#include "TouchModule.h"
#include "MotorModule.h"
#include "MqttModule.h"

#include "gui/DebugWindowGui.h"

TTGOClass *ttgo;
TFT_eSPI *tft;

DebugWindowGui *debug;

const char hostname[] = HOSTNAME;
const char wifiSsid[] = STASSID;
const char wifiPass[] = STAPSK;

WiFiModule wifi(hostname, wifiSsid, wifiPass);
TouchModule touch;
MotorModule motor(MOTOR_PIN);

MqttModule mqtt(&wifi, HOSTNAME, MQTT_HOST);

char buf[128];
double lps = .0;

void publishCommand() {
    mqtt.publish(TASMOTA_TOPIC, "TOGGLE");
}

void setup() {
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    tft = ttgo->tft;

    debug = new DebugWindowGui(ttgo, 0, 120, 240, 120);
    wifi.debug = debug;
    wifi.setup();

    wifi.onStart([](WiFiClass *wifi) { debug->println("WiFi started"); });
    wifi.onStop([](WiFiClass *wifi) { debug->println("WiFi stopped"); });
    wifi.onDisconnect([](WiFiClass *wifi) { debug->println("WiFi disconnected"); });

    wifi.onConnect([](WiFiClass *wifi) {
        debug->print("Connected to ");
        debug->println(wifi->SSID());
        debug->print("IP: ");
        debug->println(wifi->localIP().toString());
    });

    wifi.connect();
    motor.setup();
    motor.vibe(100);

    touch.setup();
    touch.onTouch([](TP_Point point) {
        motor.shortVibe();
        debug->printf("Touched at [%d,%d]\n", point.x, point.y);
        // publishCommand();
    });

    ttgo->bl->adjust(30);

    mqtt.debug = debug;
    mqtt.setup();
    mqtt.connect();

    mqtt.subscribe("env/office/temp_in", [] (char *topic, uint8_t *data, unsigned int length) {
        debug->print("Message on [");
        debug->print(topic);
        debug->print("] ");
        for (int i=0;i<length;i++) {
            debug->print((char)data[i]);
        }
        debug->println();
    });
}

void drawTime()
{
    tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    tft->drawString(buf, 5, 20, 7);
}

void drawBatteryState() {
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    sprintf(buf, "%.2fV", ttgo->power->getBattVoltage() / 1000.0);
    auto width = tft->textWidth(buf);
    tft->drawString(buf, TFT_WIDTH - width, 0, 1);
}

void drawStatusBar() {
    tft->fillRect(0, 0, 30, 10, TFT_BLACK);

    if (WiFi.isConnected()) {
        tft->setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
        tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    }
    tft->drawChar('W', 0, 0, 1);

    if (mqtt.isConnected()) {
        tft->setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
        tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    }
    tft->drawChar('M', 10, 0, 1);

    snprintf(buf, sizeof(buf), "%.2flps", lps);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->drawString(buf, 30, 0, 1);

    drawBatteryState();
}

unsigned long _nextGuiUpdate = 0;
void gui_loop() {
    if (millis() > _nextGuiUpdate) {
        drawTime();
        debug->draw();
        drawStatusBar();

        _nextGuiUpdate += 1000;
    }
}

bool isStandby = false;

void onButtonPress() {
    if (!isStandby) {
        isStandby = true;

        debug->println("Going to sleep...");
        ttgo->bl->adjust(0);
        ttgo->displaySleep();
        ttgo->closeBL();

        gpio_wakeup_enable((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
        esp_sleep_enable_gpio_wakeup();
        esp_light_sleep_start();
    } else {
        isStandby = false;

        debug->println("Waking up...");
        ttgo->openBL();
        ttgo->displayWakeup();
        ttgo->bl->adjust(30);
    }
}

void pmuLoop() {
    ttgo->power->readIRQ();
    if (ttgo->power->isPEKShortPressIRQ()) {
        onButtonPress();
    }
    ttgo->power->clearIRQ();
}

bool _timeSynched = false;
unsigned long _nextSyncCheck = 0;
void time_sync_loop() {
    if (!_timeSynched && wifi.isConnected() && millis() > _nextSyncCheck) {
        tm timeinfo;
        auto *ttgo = TTGOClass::getWatch();

        configTzTime(TZ, ntp0, ntp1, ntp2);
        if (!getLocalTime(&timeinfo)) {
            debug->println("Time not synched");
            _nextSyncCheck += 1000;
        } else {
            debug->println("Time synched");
            ttgo->rtc->syncToRtc();
            _timeSynched = true;
        }
    }
}

unsigned long loops = 0;
unsigned long _nextLpsUpdate = 1000;
unsigned long _lastLpsUpdate = 0;
void lpsUpdate()
{
    if (millis() > _nextLpsUpdate) {
        lps = (loops / (millis() - _lastLpsUpdate + .0))*1000;
        Serial.printf("%.2f lps\n", lps);
        loops = 0;
        _lastLpsUpdate = millis();
        _nextLpsUpdate = _lastLpsUpdate + 2000;
    }
}

void loop() {
    loops++;
    pmuLoop();
        wifi.update(millis());
    mqtt.update(millis());
        touch.update(millis());
        gui_loop();
        time_sync_loop();
    lpsUpdate();
}