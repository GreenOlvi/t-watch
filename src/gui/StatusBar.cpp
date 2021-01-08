#include "gui/StatusBar.h"

StatusBar::StatusBar(WatchClass *watch)
    : _watch(watch) {
}

void StatusBar::setup(TFT_eSPI *tft) {
    _buffer = new TFT_eSprite(tft);
    _buffer->createSprite(240, 10);
}

void StatusBar::update(const unsigned long t) {
    _shouldRedraw = t > _lastRedraw + 1000;
}

TFT_eSprite* StatusBar::draw() {
    if (!_shouldRedraw)
        return _buffer;

    _buffer->fillSprite(TFT_BLACK);
    _buffer->setTextFont(1);

    if (_watch->wifi->isConnected()) {
        _buffer->setTextColor(TFT_GREEN);
    } else {
        _buffer->setTextColor(TFT_LIGHTGREY);
    }
    _buffer->drawChar('W', 0, 0, 1);

    if (_watch->mqtt->isConnected()) {
        _buffer->setTextColor(TFT_GREEN);
    } else {
        _buffer->setTextColor(TFT_LIGHTGREY);
    }
    _buffer->drawChar('M', 10, 0, 1);

    char buf[20];

    snprintf(buf, sizeof(buf), "%.2f lps", _watch->loopsPerSecond);
    _buffer->setTextColor(TFT_GREEN);
    _buffer->drawString(buf, 30, 0, 1);

    _buffer->setTextColor(TFT_GREEN);
    sprintf(buf, "%.2fV", _watch->ttgo->power->getBattVoltage() / 1000.0);
    auto width = _buffer->textWidth(buf);
    _buffer->drawString(buf, TFT_WIDTH - width, 0, 1);

    _lastRedraw = millis();

    return _buffer;
}

bool StatusBar::shouldRedraw() {
    return _shouldRedraw;
}