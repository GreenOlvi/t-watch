#ifndef WiFiModule_h
#define WiFiModule_h

#include <WiFi.h>
#include <mdns.h>
#include "Module.h"

typedef std::function<void(WiFiClass *wifi)> WiFiModuleEvent;

class WiFiModule : public Module {
    public:
        Print *debug;
        WiFiModule(const char* hostname, const char* ssid, const char* password);
        void setup(void) override;
        void update(const unsigned long t) override;
        void connect(void);
        void disconnect(void);
        bool isConnected(void);
        WiFiClient client();

        bool resolveHostname(const char *hostname, IPAddress &result);

        void onStart(WiFiModuleEvent wifi);
        void onStop(WiFiModuleEvent wifi);
        void onConnect(WiFiModuleEvent wifi);
        void onDisconnect(WiFiModuleEvent wifi);

    private:
        const char* _hostname;
        const char* _ssid;
        const char* _password;
        bool _stayConnected = false;
        int _retryCount;
        unsigned long _nextRetry = 0;
        void reconnect();
        void begin();
        bool mdnsBegin();
};

#endif