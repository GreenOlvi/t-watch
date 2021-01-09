#ifndef MenuFrame_h
#define MenuFrame_h

#include "Frame.h"
#include "StatusBar.h"

#include "images/menu.h"

class MenuFrame : public Frame {
    public:
        MenuFrame(StatusBar *statusBar);
        void setup(TFT_eSPI *tft) override;
        void draw(void) override;
        void update(const unsigned long t) override;
        void unload(void) override;
        void onTouch(point_t p) override;

    private:
        TFT_eSprite *_bg;
        StatusBar *_statusBar;

        bool _drawBg = true;
};

#endif