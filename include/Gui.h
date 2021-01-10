#ifndef Gui_h
#define Gui_h

#include "common.h"
#include "Module.h"
#include "WatchClass.h"

#include "gui/Frame.h"
#include "gui/StatusBar.h"
#include "gui/ClockFrame.h"
#include "gui/MenuFrame.h"

class Gui : public Module {
    public:
        Gui(WatchClass *watch);
        void setup(void) override;
        void update(const unsigned long t) override;

        void switchFrame(int newFrame);
        void handleTouch(point_t p);

    private:
        WatchClass *_watch;
        TFT_eSPI *_tft;
        unsigned long _nextDraw = 0;

        Frame *_currentFrame;
        int _frame = 0;
        int _nextFrame = -1;

        StatusBar *_statusBar;
        ClockFrame *_clockFrame;
        MenuFrame *_menuFrame;

        void replaceCurrentFrame(int newFrame);
};

#endif