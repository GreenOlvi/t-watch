#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "secrets.h"

TTGOClass *ttgo;

const char wifiSsid[] = STASSID;
const char wifiPass[] = STAPSK;

char buf[128];
bool _timeSynched = false;

void setup() {
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    WiFi.begin(wifiSsid, wifiPass);
}

unsigned long _nextGuiUpdate = 0;
void gui_loop() {
    if (millis() > _nextGuiUpdate) {
        ttgo->tft->setTextColor(random(0xFFFF));
        ttgo->tft->drawString("T-Watch RTC",  50, 50, 4);

        ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
        snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
        ttgo->tft->drawString(buf, 5, 118, 7);

        ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);
        if (WiFi.isConnected()) {
            ttgo->tft->drawString(WiFi.localIP().toString(), 0, 230, 1);
        } else {
            ttgo->tft->drawString("No wifi :-(", 0, 230, 1);
        }

        if (_timeSynched) {
            auto textWidth = ttgo->tft->textWidth("synched", 1);
            ttgo->tft->drawString("synched", TFT_WIDTH - textWidth, 230, 1);
        }

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
            _nextSyncCheck += 1000;
        } else {
            ttgo->rtc->syncToRtc();
            _timeSynched = true;
        }
    }
}

void loop() {
    gui_loop();
    time_sync_loop();
}