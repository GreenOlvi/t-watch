#ifndef MqttModule_h
#define MqttModule_h

#include "config.h"
#include "Module.h"
#include "WiFiModule.h"
#include <PubSubClient.h>

class MqttModule : public Module {
    public:
        Print *debug;

        MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port = 1883);
        MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port = 1883);
        void setup(void) override;
        void update(const unsigned long t) override;
        void publish(const char *topic, const char *payload);
        void stayConnected(bool value);
    private:
        void connect(void);

        bool _stayConnected;
        unsigned long _lastUpdate = 0;
        const char *_clientId;
        PubSubClient _client;
        WiFiClient _wifi;
};

#endif