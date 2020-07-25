#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

#include <string.h>

class DebugWindow {
    public:
        DebugWindow(unsigned int cols, unsigned int rows);
        void println(const char* text);
        char getChar(unsigned int x, unsigned int y);

    private:
        unsigned int _cols, _rows;
        unsigned int _curX, _curY;
        char *_chars;
        unsigned int *_rowIndex;

        void shiftLinesUp(void);
};

#endif