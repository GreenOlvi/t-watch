#ifndef Frame_h
#define Frame_h

#include "config.h"

class Frame {
    public:
        virtual void setup(TFT_eSPI *tft) = 0;
        virtual void draw(void) = 0;
        virtual void update(const unsigned long t) = 0;
};

#endif