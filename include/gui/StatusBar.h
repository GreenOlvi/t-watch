#ifndef StatusBar_h
#define StatusBar_h

#include "GuiElement.h"
#include "WatchClass.h"

class StatusBar : public GuiElement {
    public:
        StatusBar(WatchClass *watch);
        void setup(TFT_eSPI *tft) override;
        void update(const unsigned long t);
        TFT_eSprite *draw() override;
        bool shouldRedraw();

        const int height = 10;

    private:
        WatchClass *_watch;
        TFT_eSprite *_buffer;
        unsigned long _lastRedraw = 0;
        bool _shouldRedraw;
};

#endif