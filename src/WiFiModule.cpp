#include "WiFiModule.h"

WiFiModule::WiFiModule(const char *hostname, const char *ssid, const char *password)
    : _hostname(hostname), _ssid(ssid), _password(password) {
}

void WiFiModule::setup() {
    WiFi.setHostname(_hostname);
    WiFi.mode(WIFI_STA);
}

void WiFiModule::connect() {
    _stayConnected = true;
}

void WiFiModule::disconnect() {
    _stayConnected = false;
}

bool WiFiModule::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

WiFiClient WiFiModule::client() {
    return WiFiClient();
}

void WiFiModule::onStart(WiFiModuleEvent moduleEvent) {
    WiFi.onEvent([moduleEvent](WiFiEvent_t event, WiFiEventInfo_t info) {
        moduleEvent(&WiFi);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_START);
}

void WiFiModule::onStop(WiFiModuleEvent moduleEvent) {
    WiFi.onEvent([moduleEvent](WiFiEvent_t event, WiFiEventInfo_t info) {
        moduleEvent(&WiFi);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_STOP);
}

void WiFiModule::onConnect(WiFiModuleEvent moduleEvent) {
    WiFi.onEvent([moduleEvent](WiFiEvent_t event, WiFiEventInfo_t info) {
        moduleEvent(&WiFi);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
}

void WiFiModule::onDisconnect(WiFiModuleEvent moduleEvent) {
    WiFi.onEvent([moduleEvent](WiFiEvent_t event, WiFiEventInfo_t info) {
        moduleEvent(&WiFi);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
}

void WiFiModule::update(const unsigned long t) {
    if (!isConnected() && _stayConnected) {
        reconnect();
    }
    else if (isConnected() && !_stayConnected)
    {
        WiFi.disconnect(true);
    }
}

bool WiFiModule::reconnect() {
    return WiFi.begin(_ssid, _password) == WL_CONNECTED;
}