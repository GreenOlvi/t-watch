#ifndef TouchModule_h
#define TouchModule_h

#include "common.h"
#include "Module.h"

typedef std::function<void(point_t)> TouchModuleEvent;

class TouchModule : public Module {
    public:
        TouchModule(void);
        void setup(void) override;
        void update(const unsigned long t) override;

        bool isTouching(void);

        void onTouch(TouchModuleEvent event);

        void enable();
        void disable();

    private:
        TTGOClass *_ttgo;
        bool _enabled;
        unsigned int _updateDelay = 50;
        unsigned long _nextUpdate = 0;
        bool _isTouched = false;

        TouchModuleEvent _onTouch = nullptr;
};

#endif