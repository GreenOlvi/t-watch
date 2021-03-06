#include <Arduino.h>

#include "WatchClass.h"
#include "Gui.h"

WatchClass *watch;
Gui *gui;

TaskHandle_t BgTask;

// void watchLoop(void *param) {
//     while (1) {
//         watch->update();
//     }
// }

void loop() {
    watch->update();
    gui->update(millis());
}

void setup() {
    Serial.begin(115200);

    watch = new WatchClass();
    watch->setup();

    gui = new Gui(watch);
    gui->setup();

    // xTaskCreate(watchLoop, "WatchTask", 10000, NULL, 0, &BgTask);

    watch->touch->onTouch([](point_t p) { gui->handleTouch(p);});
}