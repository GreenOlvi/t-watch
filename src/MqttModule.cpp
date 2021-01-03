#include "MqttModule.h"

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port) :
    _hostname(hostname), _port(port), _clientId(clientId) {
    _wifi = wifiModule->client();
    _client = PubSubClient(_wifi);
}

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port) :
    _ip(ip), _hostname(nullptr), _port(port), _clientId(clientId)  {
    _wifi = wifiModule->client();
    _client = PubSubClient(_wifi);
    _client.setServer(_ip, _port);
}

void MqttModule::setup() {
    _client.setCallback([this] (char *topic, uint8_t *data, unsigned int length) {
        this->callback(topic, data, length);
    });
}

void MqttModule::update(const unsigned long t) {
    if (!_client.connected()) {
        if (_stayConnected && t > _nextRetry) {
            if (reconnect()) {
                debug->println("MQTT connected");
                _retryCount = 0;
            } else {
                unsigned int sec = _retryCount < 12 ? pow(2, _retryCount) : 4096;
                debug->printf("MQTT connection failed. Waiting %d seconds\n", sec);
                _nextRetry = t + sec * 1000;
            }
        }
    } else {
        if (_stayConnected) {
            _client.loop();
        } else {
            _client.disconnect();
        }
    }
}

void MqttModule::publish(const char *topic, const char *payload) {
    if (_client.connected()) {
        _client.publish(topic, payload);
    } else {
        debug->println("Mqtt disconnected");
    }
}

void MqttModule::stayConnected(bool value) {
    _stayConnected = value;
    _nextRetry = 0;
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
    if (WiFi.isConnected()) {
        if (!_ip) {
            if (!resolveHostname()) {
                debug->println("Mqtt broker hostname not resolved");
                return false;
            }
            debug->println("Mqtt broker hostname resolved");
            _client.setServer(_ip, _port);
        }

        debug->println("Connecting to Mqtt broker");
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
    auto ip = MDNS.queryHost(_hostname);
    if (ip) {
        _ip = ip;
        return true;
    }
    return false;
}