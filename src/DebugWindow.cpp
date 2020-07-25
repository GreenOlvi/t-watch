#include "DebugWindow.h"

DebugWindow::DebugWindow(unsigned int cols, unsigned int rows)
    : _cols(cols), _rows(rows) {
    _chars = new char[_cols * _rows];

    memset(_chars, 0, _cols * _rows);

    _chars[0] = '0';
    _chars[_cols - 1] = '1';
    _chars[_cols * (_rows - 1)] = '2';
    _chars[_cols * _rows - 1] = '3';

    _curX = 0;
    _curY = 0;
}

void DebugWindow::println(const char* text) {
    strcpy(_chars + _curY * _cols, text);
    _curY++;

    if (_curY >= _rows) {
        _curY = 0;
    }
}

char DebugWindow::getChar(unsigned int x, unsigned int y) {
    return _chars[y * _cols + x];
}