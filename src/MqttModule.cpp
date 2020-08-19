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
    if (!_ip) {
        _ip = MDNS.queryHost(_hostname);
    }
    _client.setServer(_ip, _port);
}

void MqttModule::update(const unsigned long t) {
    if (t >= _lastUpdate) {
        if (_stayConnected && WiFi.isConnected() && !_client.connected()) {
            connect();
        }

        _lastUpdate += 10000;
    }
    _client.loop();
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

void MqttModule::connect() {
    _client.connect(_clientId);
}