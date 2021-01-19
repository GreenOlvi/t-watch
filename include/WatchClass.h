#ifndef WatchClass_h
#define WatchClass_h

#include "config.h"
#include "common.h"
#include "secrets.h"
#include "PowerModule.h"
#include "WiFiModule.h"
#include "MqttModule.h"
#include "MotorModule.h"
#include "TouchModule.h"
#include "esp_sleep.h"

class WatchClass {
    public:
        WatchClass(void);
        void setup(void);
        void update(void);

        void lightSleep();
        void wakeUp();

        bool isStandby();

        TTGOClass *ttgo;
        TFT_eSPI *tft;
        PowerModule *power;
        WiFiModule *wifi;
        MqttModule *mqtt;
        MotorModule *motor;
        TouchModule *touch;

        double loopsPerSecond = .0;

    private:
        static constexpr const char *TAG = "WatchClass";

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