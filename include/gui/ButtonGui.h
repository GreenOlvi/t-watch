#ifndef BUTTON_GUI_H
#define BUTTON_GUI_H

#include "Drawable.h"

class ButtonGui : public Drawable {
    public:
        ButtonGui(int x, int y, int width, int height, const char *label);
        void draw(TFT_eSPI *tft) override;
        bool isInside(int x, int y);

    private:
        const int _x, _y, _width, _height;
        const char *_label;
};

#endif