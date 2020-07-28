#include "WiFiModule.h"

WiFiModule::WiFiModule(const char *hostname, const char *ssid, const char *password)
    : _hostname(hostname), _ssid(ssid), _password(password) {
}

void WiFiModule::setup() {
    WiFi.setHostname(_hostname);
    WiFi.mode(WIFI_STA);
}

void WiFiModule::connect() {
    WiFi.begin(_ssid, _password);
}

void WiFiModule::disconnect() {
    WiFi.disconnect(true);
}

bool WiFiModule::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiModule::onConnect(WiFiEventFuncCb event) {
    WiFi.onEvent(event, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
}

void WiFiModule::update(const unsigned long t) {
}