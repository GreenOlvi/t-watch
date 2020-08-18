#include "MqttModule.h"

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port) :
    _clientId(clientId) {
    _wifi = wifiModule->client();
    _client = PubSubClient(hostname, port, _wifi);
}

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port) :
    _clientId(clientId) {
    _wifi = wifiModule->client();
    _client = PubSubClient(ip, port, _wifi);
}

void MqttModule::setup() {
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

void MqttModule::connect() {
    _client.connect(_clientId);
}