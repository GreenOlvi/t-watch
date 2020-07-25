#include "gui/DebugWindowGui.h"

DebugWindowGui::DebugWindowGui(TTGOClass *ttgo, int x, int y, int width, int height)
    : _ttgo(ttgo), _x(x), _y(y), _width(width), _height(height) {
    _cols = (_width - 2) / _charWidth;
    _rows = (_height - 2) / _charHeight;

    _debug = new DebugWindow(_cols, _rows);
}

void DebugWindowGui::println(const char* text) {
    _debug->println(text);
}

void DebugWindowGui::draw() {
    // _ttgo->tft->drawRect(_x, _y, _width, _height, TFT_GREEN);

    _ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);

    for (int j = 0; j < _rows; j++) {
        // _ttgo->tft->fillRect(_x + 1, _y + 4 + i * 10, _width - 2, 10, TFT_BLACK);
        // _ttgo->tft->drawString(_lines[i], _x + 3, _y + 4 + i * 10, 1);
        for (int i = 0; i < _cols; i++) {
            char c = _debug->getChar(i, j);
            _ttgo->tft->drawChar(c, _x + 3 + i * _charWidth, _y + 4 + j * _charHeight);
        }
    }

}