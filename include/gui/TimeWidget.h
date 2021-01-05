#ifndef TimeWidget_h
#define TimeWidget_h

#include "GuiElement.h"

class TimeWidget : public GuiElement {
    public:
        TimeWidget(TTGOClass *ttgo);
        void setup(TFT_eSPI *tft) override;
        TFT_eSprite* draw() override;

    private:
        TTGOClass *_ttgo;
        TFT_eSprite *_buffer;
        unsigned long _lastRedraw = 0;
};

#endif