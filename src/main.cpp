#include <Arduino.h>
#include <ESPmDNS.h>
#include "config.h"
#include "secrets.h"
#include "WiFiModule.h"
#include "TouchModule.h"
#include "MotorModule.h"
#include "MqttModule.h"

#include "gui/DebugWindowGui.h"

TTGOClass *ttgo;

DebugWindowGui *debug;

const char hostname[] = HOSTNAME;
const char wifiSsid[] = STASSID;
const char wifiPass[] = STAPSK;

WiFiModule wifi(hostname, wifiSsid, wifiPass);
TouchModule touch;
MotorModule motor(MOTOR_PIN);

MqttModule mqtt(&wifi, HOSTNAME, MQTT_HOST);

char buf[128];
bool _timeSynched = false;

void publishCommand() {
    mqtt.publish(TASMOTA_TOPIC, "TOGGLE");
    debug->printf("Mqtt connected: %s\n", mqtt.isConnected() ? "true" : "false");
}

void setup() {
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    debug = new DebugWindowGui(ttgo, 0, 120, 240, 120);
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
    MDNS.begin(HOSTNAME);
    motor.setup();
    motor.vibe(500);

    touch.setup();
    touch.onTouch([](TP_Point point) {
        motor.vibe(10);
        debug->printf("Touched at [%d,%d]\n", point.x, point.y);
        publishCommand();
    });

    ttgo->bl->adjust(30);

    mqtt.debug = debug;
    mqtt.setup();
    mqtt.stayConnected(true);

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

void drawTime() {
    if (touch.isTouching()) {
        ttgo->tft->setTextColor(TFT_BLUE, TFT_BLACK);
    } else {
        ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    }
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    ttgo->tft->drawString(buf, 5, 20, 7);
}

void drawBatteryState() {
    ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);
    ttgo->tft->fillRect(210, 0, 30, 10, TFT_BLACK);
    sprintf(buf, "%d", ttgo->power->getBattPercentage());
    auto width = ttgo->tft->textWidth(buf);
    ttgo->tft->drawString(buf, TFT_WIDTH - width, 0, 1);
}

unsigned long _nextGuiUpdate = 0;
void gui_loop() {
    if (millis() > _nextGuiUpdate) {
        drawTime();
        drawBatteryState();
        debug->draw();

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

void loop() {
    pmuLoop();
    if (!isStandby) {
        wifi.update(millis());
        touch.update(millis());
        mqtt.update(millis());
        gui_loop();
        time_sync_loop();
    }
}