#include <Arduino.h>

#include "gui/DebugWindowGui.h"
#include "gui/StatusBar.h"
#include "gui/ClockFrame.h"

#include "WatchClass.h"

WatchClass *watch;

ClockFrame *clockFrame;
StatusBar *statusBar;

void guiSetup() {
    Serial.println("statusBar");
    statusBar = new StatusBar(watch);
    statusBar->setup(watch->tft);

    Serial.println("clockFrame");
    clockFrame = new ClockFrame(watch, statusBar);
    clockFrame->setup(watch->tft);
}

void guiLoop() {
    clockFrame->update(millis());
    clockFrame->draw();
}

void setup() {
    Serial.begin(115200);

    watch = new WatchClass();
    watch->setup();

    guiSetup();
}

void loop() {
    watch->update();
    guiLoop();
}