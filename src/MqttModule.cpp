#include "MqttModule.h"

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port) :
    _state(resolve_hostname), _hostname(hostname), _port(port), _clientId(clientId) {
    _wifi = wifiModule->client();
    _client = PubSubClient(_wifi);
}

MqttModule::MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port) :
    _state(do_connect), _ip(ip), _hostname(nullptr), _port(port), _clientId(clientId)  {
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
    switch (_state) {
        case resolve_hostname:
            if (tryResolveHostname(t)) {
                _client.setServer(_ip, _port);
                _state = do_connect;
            }
            break;
        case do_connect:
            if (!_client.connected()) {
                if (_stayConnected && WiFi.isConnected() && t - _lastReconnectUpdate >= ReconnectInterval) {
                    _lastReconnectUpdate = t;
                    if (connect()) {
                        debug->println("MQTT connected");
                        if (_client.subscribe(_subscribedTopic)) {
                            debug->println("Topic subscribed");
                        }
                    }
                }
            } else {
                _client.loop();
            }
            break;
        default:
            debug->println("MqttModule is in unknown state");
            break;
    };
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
    //TODO Subscribe to more than one topic
    _subscribedTopic = topic;
    _subscribedCallback = callback;
}

bool MqttModule::tryResolveHostname(const unsigned long t) {
    if (_ip) {
        return true;
    }

    if (t - _lastReconnectUpdate >= ReconnectInterval) {
        debug->printf("Resolving broker [%s]\n", _hostname);
        _ip = MDNS.queryHost(_hostname);

        if (_ip) {
            debug->printf("Broker ip: %s\n", _ip.toString().c_str());
            return true;
        }

        debug->println("Could not resolve broker ip");
    }
    return false;
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