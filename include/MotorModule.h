#ifndef MotorModule_h
#define MotorModule_h

#include "config.h"
#include "Module.h"
#include <Ticker.h>

class MotorModule {
    public:
        MotorModule(int pin);
        ~MotorModule();
        void setup(void);
        void vibe(unsigned int t);
        void shortVibe();
    private:
        const int _pin;
        const uint8_t _channel;
        double _frequency;
        Ticker *_ticker;
};

#endif