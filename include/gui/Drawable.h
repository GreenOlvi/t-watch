#ifndef Drawable_h
#define Drawable_h

#include "config.h"

class Drawable {
    public:
        virtual void draw(TFT_eSPI *tft) = 0;
};

#endif