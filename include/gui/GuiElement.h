#ifndef GuiElement_h
#define GuiElement_h

#include "common.h"

class GuiElement {
    public:
        virtual void setup(TFT_eSPI *tft) = 0;
        virtual TFT_eSprite* draw() = 0;
        virtual void unload(void) = 0;
};

#endif