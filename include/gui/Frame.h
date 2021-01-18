#ifndef Frame_h
#define Frame_h

class Frame;

#include "common.h"
#include "FrameManager.h"

class Frame {
    public:
        virtual void setup(TFT_eSPI *tft) = 0;
        virtual void draw(void) = 0;
        virtual void update(const unsigned long t) = 0;
        virtual void unload(void) = 0;
        virtual void onTouch(point_t p) = 0;

    protected:
        Frame(FrameManager *fm) : _fm(fm) {}
        FrameManager *_fm;
};

#endif