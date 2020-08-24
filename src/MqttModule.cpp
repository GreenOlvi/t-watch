#include "MqttModule.h"

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port) :
    _hostname(hostname), _port(port), _clientId(clientId) {
    _wifi = wifiModule->client();
    _client = PubSubClient(_wifi);
}

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port) :
    _ip(ip), _hostname(nullptr), _port(port), _clientId(clientId) {
    _wifi = wifiModule->client();
    _client = PubSubClient(_wifi);
}

void MqttModule::setup() {
    if (_hostname) {
        debug->printf("Resolving broker [%s]\n", _hostname);
        _ip = MDNS.queryHost(_hostname);
    }
    debug->printf("Broker ip: %s\n", _ip.toString().c_str());
    _client.setServer(_ip, _port);
    _client.setCallback([this] (char *topic, uint8_t *data, unsigned int length) {
        this->callback(topic, data, length);
    });
}

void MqttModule::update(const unsigned long t) {
    if (!_client.connected()) {
        if (_stayConnected && WiFi.isConnected() && t - _lastReconnectUpdate >= ReconnectInterval) {
            _lastReconnectUpdate = t;
            if (connect()) {
                debug->println("MQTT connected");
                if (_client.subscribe("stat/#")) {
                    debug->println("Topic subscribed");
                }
            }
        }
    } else {
        _client.loop();
    }
}

void MqttModule::publish(const char *topic, const char *payload) {
    if (_client.connected()) {
        _client.publish(topic, payload);
    }
}

void MqttModule::stayConnected(bool value) {
    _stayConnected = value;
}

bool MqttModule::connected() {
    return _client.connected();
}

void MqttModule::subscribe(const char *topic, CallbackFn callback) {
    _client.subscribe(topic);
    //TODO Subscribe to more than one topic
    _subscribedTopic = topic;
    _subscribedCallback = callback;
}

bool MqttModule::connect() {
    return _client.connect(_clientId);
}

void MqttModule::callback(char *topic, uint8_t *data, unsigned int length) {
    if (_subscribedCallback && topicMatches(_subscribedTopic, topic)) {
        _subscribedCallback(topic, data, length);
    }
}

bool MqttModule::topicMatches(const char *subscribed, const char *topic) {
    //TODO Implement matching subscriptions with '#'
    return strcmp(subscribed, topic) == 0;
}