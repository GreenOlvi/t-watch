#ifndef WiFiModule_h
#define WiFiModule_h

#include <WiFi.h>
#include "Module.h"

class WiFiModule : public Module {
    public:
        WiFiModule(const char* hostname, const char* ssid, const char* password);
        void setup(void) override;
        void update(const unsigned long t) override;
        void connect(void);
        void disconnect(void);
        bool isConnected();
        void onConnect(WiFiEventFuncCb cbEvent);

    private:
        const char* _hostname;
        const char* _ssid;
        const char* _password;

        void onConnectedHandler();
};

#endif