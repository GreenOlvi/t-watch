#ifndef ClockFrame_h
#define ClockFrame_h

#include "Frame.h"
#include "StatusBar.h"
#include "WatchClass.h"

class ClockFrame : public Frame {
    public:
        ClockFrame(WatchClass *watch, StatusBar *_statusBar);
        void setup(TFT_eSPI *tft) override;
        void draw(void) override;
        void update(const unsigned long t) override;

    private:
        WatchClass *_watch;
        TFT_eSprite *_buff;
        StatusBar *_statusBar;

        bool _shouldRedraw;
        unsigned long _nextCheck = 0;
        RTC_Date _date;
};

#endif