#include <Arduino.h>
#include "config.h"
#include "secrets.h"
#include "WiFiModule.h"
#include "TouchModule.h"
#include "MotorModule.h"
#include "MqttModule.h"

#include "gui/DebugWindowGui.h"
#include "gui/StatusBar.h"
#include "gui/ClockFrame.h"

TTGOClass *ttgo;
TFT_eSPI *tft;

const char hostname[] = HOSTNAME;
const char wifiSsid[] = STASSID;
const char wifiPass[] = STAPSK;

WiFiModule wifi(hostname, wifiSsid, wifiPass);
TouchModule touch;
MotorModule motor(MOTOR_PIN);

MqttModule mqtt(&wifi, HOSTNAME, MQTT_HOST);

DebugWindowGui *debug;
ClockFrame *clockFrame;
StatusBar *statusBar;

char buf[128];
double lps = .0;

void publishCommand() {
    mqtt.publish(TASMOTA_TOPIC, "TOGGLE");
}

bool wifiState = true;
bool mqttState = true;

void guiSetup() {
    statusBar = new StatusBar(ttgo, &wifi, &mqtt);
    statusBar->setup(tft);

    clockFrame = new ClockFrame(ttgo, statusBar);
    clockFrame->setup(tft);
}

// unsigned long _nextGuiUpdate = 0;
void gui_loop() {
    clockFrame->update(millis());
    clockFrame->draw();
}

bool isStandby = false;

void onButtonPress()
{
    if (!isStandby) {
        isStandby = true;

        debug->println("Going to sleep...");
        ttgo->bl->adjust(0);
        ttgo->displaySleep();
        ttgo->closeBL();

        mqtt.disconnect();
        wifi.disconnect();

        gpio_wakeup_enable((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
        esp_sleep_enable_gpio_wakeup();
        esp_light_sleep_start();
    } else {
        isStandby = false;

        debug->println("Waking up...");
        ttgo->openBL();
        ttgo->displayWakeup();
        ttgo->bl->adjust(defaultBg);

        if (wifiState) {
            wifi.connect();
        }

        if (mqttState) {
            mqtt.connect();
        }
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
        // Serial.printf("%.2f lps\n", lps);
        loops = 0;
        _lastLpsUpdate = millis();
        _nextLpsUpdate = _lastLpsUpdate + 2000;
    }
}

void setup() {
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    tft = ttgo->tft;

    debug = new DebugWindowGui(0, 80, 240, 84);
    debug->setup(tft);

    wifi.debug = debug;
    wifi.setup();
    if (wifiState) {
        wifi.connect();
    }

    motor.setup();

    ttgo->bl->adjust(defaultBg);

    mqtt.debug = debug;
    mqtt.setup();
    if (mqttState) {
        mqtt.connect();
    }

    mqtt.subscribe("env/office/temp_in", [] (char *topic, uint8_t *data, unsigned int length) {
        debug->print("Message on [");
        debug->print(topic);
        debug->print("] ");
        for (int i=0;i<length;i++) {
            debug->print((char)data[i]);
        }
        debug->println();
    });

    touch.setup();
    touch.onTouch([](word x, word y) {
        motor.shortVibe();
        debug->printf("Touched at [%d,%d]\n", x, y);
    });

    guiSetup();

    motor.vibe(100);
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