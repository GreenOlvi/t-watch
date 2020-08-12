#ifndef TouchModule_h
#define TouchModule_h

#include "config.h"
#include "Module.h"

typedef std::function<void(TP_Point point)> TouchModuleEvent;

class TouchModule : public Module {
    public:
        TouchModule(void);
        void setup(void) override;
        void update(const unsigned long t) override;

        bool isTouching(void);

        void onTouch(TouchModuleEvent event);
    private:
        TTGOClass *_ttgo;
        unsigned int _updateDelay = 50;
        unsigned long _nextUpdate = 0;
        bool _isTouched = false;

        TouchModuleEvent _onTouch;
};

#endif