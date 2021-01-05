#include "gui/DebugWindowGui.h"

DebugWindowGui::DebugWindowGui(int x, int y, int width, int height)
    : _x(x), _y(y), _width(width), _height(height) {
    _cols = (_width - 2) / _charWidth;
    _rows = (_height - 2) / _charHeight;

    _debug = new DebugWindow(_cols, _rows);

    _debug->onChange([this](void) { this->_shouldRedraw = true; });
}

size_t DebugWindowGui::write(uint8_t chr) {
    Serial.write(chr);
    return _debug->write(chr);
}

void DebugWindowGui::setup(TFT_eSPI *tft) {
    _buffer = new TFT_eSprite(tft);
    _buffer->createSprite(_width, _height);
}

TFT_eSprite* DebugWindowGui::draw() {
    if (!_shouldRedraw) {
        return _buffer;
    }

    _buffer->fillSprite(TFT_BLACK);
    _buffer->setTextColor(TFT_GREEN, TFT_BLACK);
    _buffer->setTextFont(1);

    for (int j = 0; j < _rows; j++)
    {
        for (int i = 0; i < _cols; i++) {
            char c = _debug->getChar(i, j);
            if (c != 0) {
                _buffer->drawChar(c, 3 + i * _charWidth, 4 + j * _charHeight);
            }
        }
    }

    _buffer->drawRect(0, 0, _width-1, _height-1, TFT_DARKGREEN);
    return _buffer;
}