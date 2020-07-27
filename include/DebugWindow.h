#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

#include <Print.h>
#include <string.h>

class DebugWindow : public Print {
    public:
        DebugWindow(unsigned int cols, unsigned int rows);
        char getChar(unsigned int x, unsigned int y);
        size_t write(uint8_t) override;

    private:
        unsigned int _cols, _rows;
        unsigned int _curX, _curY;
        char *_chars;
        unsigned int *_rowIndex;

        void shiftLinesUp(void);
        void nextChar();
        void nextLine();
};

#endif