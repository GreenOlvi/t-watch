#include "WatchClass.h"

WatchClass::WatchClass() {}

void WatchClass::setup() {
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

    setCpuFrequencyMhz(160);

    tft = ttgo->tft;

    power = new PowerModule(ttgo->power);
    power->setup();
    power->onButtonShortPress([this]() { onButtonPress(); });

    wifi = new WiFiModule(HOSTNAME, STASSID, STAPSK);
    wifi->setup();

    mqtt = new MqttModule(wifi, HOSTNAME, MQTT_HOST);
    mqtt->setup();

    motor = new MotorModule(MOTOR_PIN);
    motor->setup();

    touch = new TouchModule();
    touch->setup();

    // TODO Store config
    if (_wifiState) {
        wifi->connect();
    }

    if (_mqttState) {
        mqtt->connect();
    }

    // FIXME Crashing when not subscribed to any topic
    mqtt->subscribe("env/office/temp_in", [this](char *topic, uint8_t *data, unsigned int length) {
        auto s = new char[length + 1];
        memcpy(s, data, length);
        s[length] = 0;
        ESP_LOGI(TAG, "Message on [%s] %s", topic, s);
    });

    ttgo->openBL();
    delay(5);
    ttgo->setBrightness(30);
}

void WatchClass::update() {
    unsigned long const t = millis();
    power->update(t);
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
        lightSleep();
    } else {
        wakeUp();
    }
}

void WatchClass::lightSleep() {
    _isStandby = true;

    ESP_LOGI(TAG, "Going to light sleep...");

    touch->disable();

    ttgo->closeBL();
    ttgo->displaySleep();

    wifi->disconnect();

    setCpuFrequencyMhz(10);
    gpio_wakeup_enable((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
}

void WatchClass::wakeUp() {
    _isStandby = false;

    setCpuFrequencyMhz(160);
    ESP_LOGI(TAG, "Waking up...");

    ttgo->displayWakeup();
    ttgo->openBL();
    ttgo->bl->adjust(30);

    touch->enable();

    if (_wifiState) {
        wifi->connect();
    }

    if (_mqttState) {
        mqtt->connect();
    }
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
        loops = 0;
        _lastLpsUpdate = millis();
        _nextLpsUpdate = _lastLpsUpdate + 2000;
    }
}