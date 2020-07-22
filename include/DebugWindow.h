#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

#include "config.h"

#include <vector>
#include <TTGO.h>

class DebugWindow {
    public:
        DebugWindow(TTGOClass *ttgo, int x, int y, int width, int height);
        void println(const char* text);
        void draw();

    private:
        TTGOClass *_ttgo;
        int _x, _y, _width, _height;
        bool _redrawNeeded = true;

        static const unsigned int _maxLines = 11;
        unsigned int _currentLine = 0;
        char *_lines[_maxLines];
};

#endif