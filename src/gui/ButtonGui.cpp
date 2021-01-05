#include "gui/ButtonGui.h"

ButtonGui::ButtonGui(int x, int y, int width, int height, const char *label) 
    : _x(x), _y(y), _width(width), _height(height), _label(label) {
}

void ButtonGui::draw(TFT_eSPI *tft) {
    tft->drawRect(_x, _y, _width, _height, TFT_GREEN);
    tft->drawCentreString(_label, _x + _width / 2, _y + _height / 2, 1);
}

bool ButtonGui::isInside(int x, int y) {
    return x >= _x && y >= _y && x <= _x + _width && y <= _y + _height;
}