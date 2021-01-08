#ifndef WatchClass_h
#define WatchClass_h

#include "config.h"
#include "secrets.h"
#include "WiFiModule.h"
#include "MqttModule.h"
#include "MotorModule.h"
#include "TouchModule.h"
#include "DebugWindow.h"

class WatchClass {
    public:
        WatchClass(void);
        void setup(void);
        void update(void);

        TTGOClass *ttgo;
        TFT_eSPI *tft;
        WiFiModule *wifi;
        MqttModule *mqtt;
        MotorModule *motor;
        TouchModule *touch;

        Print *debug;

        double loopsPerSecond = .0;

    private:
        bool _wifiState = true;
        bool _mqttState = true;

        bool _isStandby = false;
        void pmuLoop();
        void onButtonPress();

        bool _timeSynched = false;
        unsigned long _nextSyncCheck = 0;
        void timeSynchLoop();

        unsigned long loops = 0;
        unsigned long _nextLpsUpdate = 1000;
        unsigned long _lastLpsUpdate = 0;
        void lpsUpdate();
};

#endif