#ifndef DEBUG_WINDOW_GUI_H
#define DEBUG_WINDOW_GUI_H

#include "config.h"
#include <TTGO.h>
#include "DebugWindow.h"

class DebugWindowGui {
    public:
        DebugWindowGui(TTGOClass *ttgo, int x, int y, int width, int height);
        void println(const char* text);
        void draw();

    private:
        TTGOClass *_ttgo;
        DebugWindow *_debug;

        int _x, _y, _width, _height, _cols, _rows;

        static const unsigned int _charWidth = 6;
        static const unsigned int _charHeight = 9;
};

#endif