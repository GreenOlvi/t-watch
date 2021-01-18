#include "gui/MenuFrame.h"

static const char *TAG = "MenuFrame";

MenuFrame::MenuFrame(FrameManager *fm, WatchClass *watch, StatusBar *statusBar) 
    : Frame(fm), _watch(watch), _statusBar(statusBar) {
}

void MenuFrame::setup(TFT_eSPI *tft) {
    _bg = new TFT_eSprite(tft);
    _bg->createSprite(240, 230);

    _bg->drawBitmap(0, 0, image_data_menu, 240, 230, TFT_GREEN);

    _buttonCount = 4;
    _buttons = new rect_t[_buttonCount];

    for (int i = 0; i < 4; i++) {
        int x = 12 + i * 55;
        int y = 32;
        _bg->drawRoundRect(x, y, 45, 45, 5, TFT_GREEN);
        _buttons[i] = { .x = x, .y = y, .w = 45, .h = 45 };
    }
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
    _statusBar->update(t);
}

void MenuFrame::unload() {
    _bg->deleteSprite();
    _drawBg = true;
}

void MenuFrame::onTouch(point_t p) {
    int btn;
    if (buttonPressed(p, btn)) {
        switch(btn) {
            case 0:
                wifiToggle();
                break;
            case 1:
                mqttToggle();
                break;
        }
        return;
    }

    if (p.y > 200) {
        _fm->switchFrame(0);
    }
}

bool MenuFrame::buttonPressed(point_t p, int &button) {
    for (int i = 0; i <= _buttonCount; i++) {
        if (isInside(_buttons[i], p)) {
            Serial.printf("Button %d pressed\n", i);
            button = i;
            return true;
        }
    }
    Serial.printf("No button pressed\n");
    return false;
}

void MenuFrame::wifiToggle() {
    if (_watch->wifi->isConnected()) {
        _watch->wifi->disconnect();
    } else {
        _watch->wifi->connect();
    }
}

void MenuFrame::mqttToggle() {
    if (_watch->mqtt->isConnected()) {
        _watch->mqtt->disconnect();
    } else {
        _watch->mqtt->connect();
    }
}