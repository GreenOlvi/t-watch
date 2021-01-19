#ifndef PowerModule_h
#define PowerModule_h

#include "common.h"
#include "Module.h"

typedef std::function<void(void)> PowerModuleEvent;

class PowerModule : public Module {
    public:
        PowerModule(AXP20X_Class *power);
        void setup(void) override;
        void update(const unsigned long t) override;

        void onButtonShortPress(PowerModuleEvent handler);

        bool isCharging();
        float getBatteryVoltage();
        int getBatteryPercentage();

    private:

        AXP20X_Class *_power;
        bool _irq;
        bool _isCharging;

        bool _hasOnButtonShortPress;
        PowerModuleEvent _onButtonShortPressHandler;

        static PowerModule *_instance;
        static void InterruptHandler(void);
};

#endif