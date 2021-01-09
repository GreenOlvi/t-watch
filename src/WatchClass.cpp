#include "WatchClass.h"

WatchClass::WatchClass() {}

void WatchClass::setup() {
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    tft = ttgo->tft;

    debug = new DebugWindow(40, 40);

    wifi = new WiFiModule(HOSTNAME, STASSID, STAPSK);
    wifi->debug = debug;
    wifi->setup();

    mqtt = new MqttModule(wifi, HOSTNAME, MQTT_HOST);
    mqtt->debug = debug;
    mqtt->setup();

    motor = new MotorModule(MOTOR_PIN);
    motor->setup();

    touch = new TouchModule();
    touch->setup();

    // TODO Store config
    ttgo->bl->adjust(30);

    if (_wifiState) {
        wifi->connect();
    }

    if (_mqttState) {
        mqtt->connect();
    }

    // FIXME Crashing when not subscribed to any topic
    mqtt->subscribe("env/office/temp_in", [this] (char *topic, uint8_t *data, unsigned int length) {
        debug->print("Message on [");
        debug->print(topic);
        debug->print("] ");
        for (int i=0;i<length;i++) {
            debug->print((char)data[i]);
        }
        debug->println();
    });
}

void WatchClass::update() {
    unsigned long const t = millis();
    pmuLoop();
    wifi->update(t);
    mqtt->update(t);
    touch->update(t);
    timeSynchLoop();
    lpsUpdate();
}

bool WatchClass::isStandby() {
    return _isStandby;
}

void WatchClass::onButtonPress()
{
    if (!_isStandby) {
        _isStandby = true;

        touch->disable();

        debug->println("Going to sleep...");
        ttgo->bl->adjust(0);
        ttgo->displaySleep();
        ttgo->closeBL();

        mqtt->disconnect();
        wifi->disconnect();

        gpio_wakeup_enable((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
        esp_sleep_enable_gpio_wakeup();
        esp_light_sleep_start();
    } else {
        _isStandby = false;

        debug->println("Waking up...");
        ttgo->openBL();
        ttgo->displayWakeup();
        ttgo->bl->adjust(30);

        touch->enable();

        if (_wifiState) {
            wifi->connect();
        }

        if (_mqttState) {
            mqtt->connect();
        }
    }
}

void WatchClass::pmuLoop() {
    ttgo->power->readIRQ();
    if (ttgo->power->isPEKShortPressIRQ()) {
        onButtonPress();
    }
    ttgo->power->clearIRQ();
}

void WatchClass::timeSynchLoop() {
    if (!_timeSynched && wifi->isConnected() && millis() > _nextSyncCheck) {
        tm timeinfo;
        auto *ttgo = TTGOClass::getWatch();

        configTzTime(TZ, ntp0, ntp1, ntp2);
        if (!getLocalTime(&timeinfo)) {
            _nextSyncCheck += 1000;
        } else {
            ttgo->rtc->syncToRtc();
            _timeSynched = true;
        }
    }
}

void WatchClass::lpsUpdate()
{
    loops++;
    if (millis() > _nextLpsUpdate) {
        loopsPerSecond = (loops / (millis() - _lastLpsUpdate + .0))*1000;
        // Serial.printf("%.2f lps\n", lps);
        loops = 0;
        _lastLpsUpdate = millis();
        _nextLpsUpdate = _lastLpsUpdate + 2000;
    }
}