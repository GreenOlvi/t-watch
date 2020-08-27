#ifndef MqttModule_h
#define MqttModule_h

#include "config.h"
#include "Module.h"
#include "WiFiModule.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include <math.h>

typedef std::function<void(char*, uint8_t*, unsigned int)> CallbackFn;

class MqttModule : public Module {
    public:
        Print *debug;
        MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port = 1883);
        MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port = 1883);
        void setup(void) override;
        void update(const unsigned long t) override;
        void publish(const char *topic, const char *payload);
        void stayConnected(bool value);
        bool connected();
        void subscribe(const char *topic, CallbackFn callback);
    private:
        enum State {
            unknown,
            resolve_hostname,
            do_connect,
        };

        bool tryResolveHostname(const unsigned long t);
        bool connect(void);
        void callback(char *topic, uint8_t *data, unsigned int length);
        bool topicMatches(const char *subscribed, const char *topic);

        State _state;
        IPAddress _ip;
        const char *_hostname;
        uint16_t _port;
        bool _stayConnected;
        unsigned long _lastResolveUpdate = 0;
        unsigned long _lastReconnectUpdate = 0;

        unsigned long _reconnectInterval = 1000;
        unsigned int _retryCount = 0;

        const char *_clientId;
        PubSubClient _client;
        WiFiClient _wifi;

        const char *_subscribedTopic;
        CallbackFn _subscribedCallback;
};

#endif