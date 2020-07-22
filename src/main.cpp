#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "secrets.h"

#include "DebugWindow.h"

TTGOClass *ttgo;

DebugWindow *debug;

const char wifiSsid[] = STASSID;
const char wifiPass[] = STAPSK;

char buf[128];
bool _timeSynched = false;

void setup() {
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    debug = new DebugWindow(ttgo, 0, 120, 240, 120);

    WiFi.begin(wifiSsid, wifiPass);
}

unsigned long _nextGuiUpdate = 0;
void gui_loop() {
    if (millis() > _nextGuiUpdate) {
        ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
        snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
        ttgo->tft->drawString(buf, 5, 20, 7);

        snprintf(buf, sizeof(buf), "Current time: %s", ttgo->rtc->formatDateTime());
        debug->println(buf);

        debug->draw();

        _nextGuiUpdate += 1000;
    }
}

unsigned long _nextSyncCheck = 0;
void time_sync_loop() {
    if (!_timeSynched && WiFi.isConnected() && millis() > _nextSyncCheck) {
        tm timeinfo;
        auto *ttgo = TTGOClass::getWatch();

        configTzTime(TZ, ntp0, ntp1, ntp2);
        if (!getLocalTime(&timeinfo)) {
            debug->println("Time not synched");
            _nextSyncCheck += 1000;
        } else {
            debug->println("Time synched");
            ttgo->rtc->syncToRtc();
            _timeSynched = true;
        }
    }
}

void loop() {
    gui_loop();
    time_sync_loop();
}