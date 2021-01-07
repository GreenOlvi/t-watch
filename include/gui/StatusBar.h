#ifndef StatusBar_h
#define StatusBar_h

#include "GuiElement.h"
#include "WiFiModule.h"
#include "MqttModule.h"

class StatusBar : public GuiElement {
    public:
        StatusBar(TTGOClass *ttgo, WiFiModule *wifi, MqttModule *mqtt);
        void setup(TFT_eSPI *tft) override;
        void update(const unsigned long t);
        TFT_eSprite *draw() override;
        bool shouldRedraw();

        const int Height = 10;

    private:
        TTGOClass *_ttgo;
        TFT_eSprite *_buffer;
        WiFiModule *_wifi;
        MqttModule *_mqtt;
        unsigned long _lastRedraw = 0;
        bool _shouldRedraw;
};

#endif