#include "gui/ClockFrame.h"

#define DATES_EQUAL(a, b) (a.second == b.second && a.minute == b.minute && a.hour == b.hour && a.day == b.day && a.month == b.month && a.year == b.year)

ClockFrame::ClockFrame(WatchClass *watch, StatusBar *statusBar) 
    : _watch(watch), _statusBar(statusBar) {
}

void ClockFrame::setup(TFT_eSPI *tft) {
    _buff = new TFT_eSprite(tft);
    _buff->createSprite(240, 240 - _statusBar->height);
}

void ClockFrame::draw() {
    if (!_shouldRedraw)
        return;

    _buff->fillSprite(TFT_BLACK);
    _buff->setTextFont(7);
    _buff->setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    _buff->setTextDatum(MC_DATUM);

    _buff->drawString(_watch->ttgo->rtc->formatDateTime(), 120, 110, 7);

    _buff->pushSprite(0, _statusBar->height);
    _statusBar->draw()->pushSprite(0, 0);

    _shouldRedraw = false;
}

void ClockFrame::update(const unsigned long t) {
    if (t < _nextCheck)
        return;

    RTC_Date dt = _watch->ttgo->rtc->getDateTime();
    if (!DATES_EQUAL(_date, dt)) {
        _date = dt;
        _shouldRedraw = true;
    }

    _statusBar->update(t);
    _shouldRedraw |= _statusBar->shouldRedraw();

    _nextCheck = t + 100;
}