#include "gui/MenuFrame.h"

static const char *TAG = "MenuFrame";

MenuFrame::MenuFrame(FrameManager *fm, WatchClass *watch, StatusBar *statusBar) 
    : Frame(fm), _watch(watch), _statusBar(statusBar) {
}

void MenuFrame::setup(TFT_eSPI *tft) {
    _bg = new TFT_eSprite(tft);
    _bg->createSprite(240, 230);

    _bg->drawBitmap(0, 0, image_data_menu, 240, 230, TFT_GREEN);
    _closeButton = rectFromAbsolute(207, 0, 238, _statusBar->height + 23);

    drawButtons();
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

void MenuFrame::drawButtons() {
    int buttonCols = 3;
    int buttonRows = 3;

    rect_t buttonArea = rectFromAbsolute(2, 24, 237, 227);

    _buttonCount = buttonRows * buttonCols;
    _buttons = new rect_t[_buttonCount];

    int buttonWidth = buttonArea.w / buttonCols;
    int buttonHeight = buttonArea.h / buttonRows;

    for (int j = 0; j < buttonRows; j++) {
        for (int i = 0; i < buttonCols; i++)
        {
            int x = buttonArea.x + i * buttonWidth;
            int y = buttonArea.y + j * buttonHeight;

            int index = j * buttonCols + i;
            _buttons[index] = {.x = x, .y = y, .w = buttonWidth, .h = buttonHeight};
            drawButton(index, _buttons[index]);
        }
    }
}

void MenuFrame::drawButton(int i, rect_t rect) {
    point_t buttonCentre = rectGetCentre(rect);
    _bg->drawRoundRect(rect.x + 4, rect.y + 4, rect.w - 8, rect.h - 8, 5, TFT_GREEN);
    _bg->setTextFont(1);
    _bg->setTextColor(TFT_WHITE);
    _bg->drawCentreString(String(i), buttonCentre.x, buttonCentre.y, 1);
}

void MenuFrame::update(const unsigned long t) {
    _statusBar->update(t);
}

void MenuFrame::unload() {
    _bg->deleteSprite();
    _drawBg = true;
}

void MenuFrame::onTouch(point_t p) {
    point_t rel = {.x = p.x, .y = p.y - _statusBar->height};

    int btn;
    if (buttonPressed(rel, btn)) {
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

    if (isInside(_closeButton, p)) {
        _fm->switchFrame(0);
    }
}

bool MenuFrame::buttonPressed(point_t p, int &button) {
    for (int i = 0; i <= _buttonCount; i++) {
        if (isInside(_buttons[i], p)) {
            ESP_LOGD(TAG, "Button %d pressed\n", i);
            button = i;
            return true;
        }
    }
    ESP_LOGD(TAG, "No button pressed\n");
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