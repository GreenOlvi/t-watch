#include "gui/TimeWidget.h"

TimeWidget::TimeWidget(TTGOClass *ttgo)
    : _ttgo(ttgo) {
}

void TimeWidget::setup(TFT_eSPI *tft) {
    _buffer = new TFT_eSprite(tft);
    _buffer->createSprite(240, 50);
}

TFT_eSprite* TimeWidget::draw() {
    _buffer->fillSprite(TFT_BLACK);
    _buffer->setTextFont(7);
    _buffer->setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    _buffer->drawCentreString(_ttgo->rtc->formatDateTime(), 120, 0, 7);
    return _buffer;
}