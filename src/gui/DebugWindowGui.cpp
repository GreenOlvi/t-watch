#include "gui/DebugWindowGui.h"

DebugWindowGui::DebugWindowGui(int x, int y, int width, int height)
    : _x(x), _y(y), _width(width), _height(height) {
    _cols = (_width - 2) / _charWidth;
    _rows = (_height - 2) / _charHeight;

    _debug = new DebugWindow(_cols, _rows);
}

size_t DebugWindowGui::write(uint8_t chr) {
    return _debug->write(chr);
}

void DebugWindowGui::draw(TFT_eSPI *tft) {
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->drawRect(_x, _y, _width, _height, TFT_DARKGREEN);

    for (int j = 0; j < _rows; j++)
    {
        for (int i = 0; i < _cols; i++) {
            char c = _debug->getChar(i, j);
            if (c == 0) {
                tft->drawChar(' ', _x + 3 + i * _charWidth, _y + 4 + j * _charHeight);
            } else {
                tft->drawChar(c, _x + 3 + i * _charWidth, _y + 4 + j * _charHeight);
            }
        }
    }
}