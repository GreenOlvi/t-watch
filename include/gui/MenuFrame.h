#ifndef MenuFrame_h
#define MenuFrame_h

#include "Frame.h"
#include "StatusBar.h"
#include "WatchClass.h"

#include "images/menu.h"

class MenuFrame : public Frame {
    public:
        MenuFrame(FrameManager *fm, WatchClass *watch, StatusBar *statusBar);
        void setup(TFT_eSPI *tft) override;
        void draw(void) override;
        void update(const unsigned long t) override;
        void unload(void) override;
        void onTouch(point_t p) override;

    private:
        WatchClass *_watch;
        TFT_eSprite *_bg;
        StatusBar *_statusBar;

        bool _drawBg = true;

        rect_t *_buttons;
        int _buttonCount;

        bool buttonPressed(point_t p, int &button);

        void wifiToggle();
        void mqttToggle();
        void lampToggle();
};

#endif