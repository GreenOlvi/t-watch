#include "WiFiModule.h"

WiFiModule::WiFiModule(const char *hostname, const char *ssid, const char *password)
    : _hostname(hostname), _ssid(ssid), _password(password) {
}

void WiFiModule::setup() {
    WiFi.mode(WIFI_OFF);
    WiFi.setHostname(_hostname);
    WiFi.persistent(false);

    onConnect([this](WiFiClass *wifi) { this->mdnsBegin(); });
}

void WiFiModule::connect() {
    _stayConnected = true;
    _retryCount = 0;
    begin();
    _nextRetry = millis() + 5000;
}

void WiFiModule::disconnect() {
    ESP_LOGI(TAG, "Disconnecting WiFi");
    _stayConnected = false;
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    _nextRetry = -1;
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
    if (t >= _nextUpdate) {
        _nextUpdate = t + 100;
        if (_stayConnected && !isConnected() && t > _nextRetry)
        {
            reconnect();
        }
    }
}

void WiFiModule::reconnect() {
    wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED)
        return;

    _retryCount++;

    if (_retryCount == 2) {
        ESP_LOGW(TAG, "WiFi not connected, restarting");
        WiFi.disconnect();
        delay(10);
        begin();
        _nextRetry = millis() + 25000;
    }

    if (_retryCount == 3) {
        ESP_LOGE(TAG, "WiFi still not connected, stopping");
        disconnect();
    }
}

void WiFiModule::begin() {
    ESP_LOGI(TAG, "Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
}

bool WiFiModule::resolveHostname(const char *hostname, IPAddress &result) {
    ESP_LOGV(TAG, "Query A: %s.local", hostname);

    struct ip4_addr addr;
    addr.addr = 0;

    esp_err_t err = mdns_query_a(hostname, 2000,  &addr);
    if(err){
        if (err == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGW(TAG, "Host was not found!");
            return false;
        }
        ESP_LOGW(TAG, "Query Failed: %d", err);
        return false;
    }

    ESP_LOGI(TAG, "Resolved '%s.local' -> %d.%d.%d.%d", hostname, IP2STR(&addr));
    result = IPAddress(addr.addr);
    return true;
}

bool WiFiModule::mdnsBegin() {
    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGW(TAG, "MDNS Init failed: %d", err);
        return false;
    }

    err = mdns_hostname_set(_hostname);
    if (err) {
        ESP_LOGW(TAG, "MDNS hostname set failed: %d", err);
        return false;
    }

    err = mdns_instance_name_set("T-Watch 2020");
    if (err) {
        ESP_LOGW(TAG, "MDNS instance name set failed: %d", err);
        return false;
    }

    ESP_LOGD(TAG, "MDNS started");

    return true;
}