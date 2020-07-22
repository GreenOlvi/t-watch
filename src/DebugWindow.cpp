#include "DebugWindow.h"

DebugWindow::DebugWindow(TTGOClass *ttgo, int x, int y, int width, int height) :
    _ttgo(ttgo), _x(x), _y(y), _width(width), _height(height)  {
}

void DebugWindow::println(const char* text) {
    char *copy = new char[strlen(text) + 1];
    strcpy(copy, text);
    
    if (_currentLine < _maxLines) {
        _lines[_currentLine] = copy;
        _currentLine++;
    } else {
        free(_lines[0]);

        // shift lines
        for (int i = 0; i < _maxLines - 1; i++) {
            _lines[i] = _lines[i + 1];
        }

        _lines[_maxLines - 1] = copy;
    }

    _redrawNeeded = true;
}

void DebugWindow::draw() {
    if (_redrawNeeded) {
        _ttgo->tft->drawRect(_x, _y, _width, _height, TFT_GREEN);
        // _ttgo->tft->fillRect(_x + 1, _y + 1, _width - 2, _height - 2, TFT_BLACK);

        _ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);

        for (int i = 0; i < _currentLine; i++) {
            _ttgo->tft->fillRect(_x + 1, _y + 4 + i * 10, _width - 2, 10, TFT_BLACK);
            _ttgo->tft->drawString(_lines[i], _x + 3, _y + 4 + i * 10, 1);
        }

        _redrawNeeded = false;
    }
}