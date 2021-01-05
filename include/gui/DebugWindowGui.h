#ifndef DEBUG_WINDOW_GUI_H
#define DEBUG_WINDOW_GUI_H

#include "config.h"
#include <Print.h>
#include "GuiElement.h"
#include "DebugWindow.h"

class DebugWindowGui : public Print, public GuiElement {
    public:
        DebugWindowGui(int x, int y, int width, int height);
        void setup(TFT_eSPI *tft) override;
        TFT_eSprite* draw() override;
        size_t write(uint8_t) override;

    private:
        DebugWindow *_debug;
        TFT_eSprite *_buffer;

        int _x, _y, _width, _height, _cols, _rows;
        bool _shouldRedraw = true;

        static const unsigned int _charWidth = 6;
        static const unsigned int _charHeight = 9;
};

#endif