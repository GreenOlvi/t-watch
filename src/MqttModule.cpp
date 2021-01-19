#include "MqttModule.h"

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port) :
    _hostname(hostname), _port(port), _clientId(clientId) {
    _wifi = wifiModule;
    _wifiClient = _wifi->client();
    _client = PubSubClient(_wifiClient);
}

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port) :
    _ip(ip), _hostname(nullptr), _port(port), _clientId(clientId)  {
    _wifi = wifiModule;
    _wifiClient = _wifi->client();
    _client = PubSubClient(_wifiClient);
    _client.setServer(_ip, _port);
}

void MqttModule::setup() {
    _client.setCallback([this](char *topic, uint8_t *data, unsigned int length) {
        this->callback(topic, data, length);
    });
}

void MqttModule::update(const unsigned long t) {
    if (t >= _nextUpdate) {
        _nextUpdate = t + 1000;
        if (!_client.connected()) {
            if (_stayConnected && t > _nextRetry) {
                if (reconnect()) {
                    ESP_LOGD(TAG, "MQTT connected");
                    _retryCount = 0;
                } else if (!_wifi->isConnected()) {
                    ESP_LOGD(TAG, "MQTT waiting for WiFi to connect");
                    _nextRetry = -1;
                    _wifi->onConnect([this](WiFiClass *wifi) { this->_nextRetry = millis(); });
                } else {
                    unsigned int sec = _retryCount < 12 ? pow(2, _retryCount) : 4096;
                    ESP_LOGW(TAG, "MQTT connection failed. Waiting %d seconds\n", sec);
                    _retryCount++;
                    _nextRetry = t + sec * 1000;
                }
            }
        }
        else
        {
            if (_stayConnected) {
                _client.loop();
            } else {
                _client.disconnect();
            }
        }
    }
}

bool MqttModule::publish(const char *topic, const char *payload) {
    ESP_LOGV(TAG, "Mqtt publish");

    if (_client.connected())
    {
        ESP_LOGI(TAG, "Mqtt publishing to '%s' [%s]\n", topic, payload);
        return _client.publish(topic, payload);
    } 

    ESP_LOGW(TAG, "Mqtt client not connected\n");
    return false;
}

void MqttModule::connect() {
    _stayConnected = true;
    _nextRetry = 0;
}

void MqttModule::disconnect() {
    _stayConnected = false;
}


bool MqttModule::isConnected() {
    return _client.connected();
}

void MqttModule::subscribe(const char *topic, CallbackFn callback) {
    //TODO Subscribe to more than one topic
    _subscribedTopic = topic;
    _subscribedCallback = callback;
}

bool MqttModule::reconnect() {
    if (_wifi->isConnected()) {
        if (!_ip) {
            if (!resolveHostname()) {
                ESP_LOGE(TAG, "Mqtt broker hostname not resolved");
                return false;
            }
            ESP_LOGV(TAG, "Mqtt broker hostname resolved");
            _client.setServer(_ip, _port);
        }

        if (_client.connect(_clientId)) {
            resubscribe();
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool MqttModule::resubscribe() {
    if (!_client.connected())
        return false;

    if (!_subscribedTopic)
        return false;

    return _client.subscribe(_subscribedTopic);
}

void MqttModule::callback(char *topic, uint8_t *data, unsigned int length) {
    if (_subscribedCallback && topicMatches(_subscribedTopic, topic)) {
        _subscribedCallback(topic, data, length);
    }
}

bool MqttModule::topicMatches(const char *subscribed, const char *topic) {
    return strcmp(subscribed, topic) == 0;
}

bool MqttModule::resolveHostname() {
    IPAddress ip;
    if (_wifi->resolveHostname(_hostname, ip)) {
        _ip = ip;
        return true;
    }
    return false;
}
