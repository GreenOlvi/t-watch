#ifndef DEBUG_WINDOW_GUI_H
#define DEBUG_WINDOW_GUI_H

#include "config.h"
#include <Print.h>
#include "Drawable.h"
#include "DebugWindow.h"

class DebugWindowGui : public Print, public Drawable {
    public:
        DebugWindowGui(int x, int y, int width, int height);
        void draw(TFT_eSPI *tft) override;
        size_t write(uint8_t) override;

    private:
        DebugWindow *_debug;

        int _x, _y, _width, _height, _cols, _rows;

        static const unsigned int _charWidth = 6;
        static const unsigned int _charHeight = 9;
};

#endif