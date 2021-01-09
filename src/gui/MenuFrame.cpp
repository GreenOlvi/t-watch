#include "gui/MenuFrame.h"

MenuFrame::MenuFrame(StatusBar *statusBar) 
    : _statusBar(statusBar) {
}

void MenuFrame::setup(TFT_eSPI *tft) {
    _bg = new TFT_eSprite(tft);
    _bg->createSprite(240, 230);

    _bg->drawBitmap(0, 0, image_data_menu, 240, 230, TFT_GREEN);
}

void MenuFrame::draw() {
    if (_drawBg) {
        _bg->pushSprite(0, 10);
        _drawBg = false;
    }

    if (_statusBar->shouldRedraw()) {
        _statusBar->draw()->pushSprite(0, 0);
    }
}

void MenuFrame::update(const unsigned long t) {

}

void MenuFrame::unload() {
    _bg->deleteSprite();
    _drawBg = true;
}

void MenuFrame::onTouch(point_t p) {
}