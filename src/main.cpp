#include <Arduino.h>
#include "config.h"

TTGOClass *ttgo;

char buf[128];

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
}

unsigned long _nextGuiUpdate = 0;
void gui_loop() {
    if (millis() > _nextGuiUpdate) {
        ttgo->tft->setTextColor(random(0xFFFF));
        ttgo->tft->drawString("T-Watch RTC",  50, 50, 4);

        ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
        snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
        ttgo->tft->drawString(buf, 5, 118, 7);
        _nextGuiUpdate += 1000;
    }
}

void loop()
{
    gui_loop();
}