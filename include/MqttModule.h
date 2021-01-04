#ifndef MqttModule_h
#define MqttModule_h

#include "config.h"
#include "Module.h"
#include "WiFiModule.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include <math.h>

typedef std::function<void(char*, uint8_t*, unsigned int)> CallbackFn;

#define DefaultMqttPort 1883
#define ReconnectInterval 10000

typedef std::function<void(char*, uint8_t*, unsigned int)> CallbackFn;

class MqttModule : public Module {
    public:
        Print *debug;
        MqttModule(WiFiModule *wifiModule, const char *clientId, const char *hostname, uint16_t port = DefaultMqttPort);
        MqttModule(WiFiModule *wifiModule, const char *clientId, const IPAddress ip, uint16_t port = DefaultMqttPort);
        void setup(void) override;
        void update(const unsigned long t) override;
        void publish(const char *topic, const char *payload);
        void stayConnected(bool value);
        void connect();
        void disconnect();
        bool isConnected();
        void subscribe(const char *topic, CallbackFn callback);
    private:
        bool reconnect(void);
        bool resubscribe(void);
        void callback(char *topic, uint8_t *data, unsigned int length);
        bool topicMatches(const char *subscribed, const char *topic);
        bool resolveHostname(void);

        IPAddress _ip;
        const char *_hostname;
        uint16_t _port;
        bool _stayConnected = false;
        int _retryCount = 0;
        unsigned long _nextRetry = 0;
        const char *_clientId;
        PubSubClient _client;
        WiFiClient _wifiClient;
        WiFiModule *_wifi;

        const char *_subscribedTopic;
        CallbackFn _subscribedCallback;
};

#endif