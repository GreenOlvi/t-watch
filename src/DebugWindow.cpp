#include "DebugWindow.h"

DebugWindow::DebugWindow(unsigned int cols, unsigned int rows)
    : _cols(cols), _rows(rows) {
    _chars = new char[_cols * _rows];
    _rowIndex = new unsigned int[_rows];

    for (int i = 0; i < _rows; i++) {
        _rowIndex[i] = i;
    }

    memset(_chars, 0, _cols * _rows);

    _chars[0] = '0';
    _chars[_cols - 1] = '1';
    _chars[_cols * (_rows - 1)] = '2';
    _chars[_cols * _rows - 1] = '3';

    _curX = 0;
    _curY = 0;
}

void DebugWindow::println(const char* text) {
    if (_curY >= _rows) {
        _curY--;
        shiftLinesUp();
    }

    strcpy(_chars + _rowIndex[_curY] * _cols, text);
    _curY++;
    _curX = 0;
}

void DebugWindow::shiftLinesUp() {
    for (int i = 0; i < _rows; i++) {
        if (_rowIndex[i] < _rows - 1) {
            _rowIndex[i] = _rowIndex[i] + 1;
        }
        else
        {
            _rowIndex[i] = 0;
        }
    }

    memset(_chars + _rowIndex[_rows - 1] * _cols, 0, _cols);
}

char DebugWindow::getChar(unsigned int x, unsigned int y) {
    if (x >= _cols || y >= _rows) {
        return '\0';
    }
    return _chars[_rowIndex[y] * _cols + x];
}