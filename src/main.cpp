#include <Arduino.h>

#include "gui/Frame.h"
#include "gui/StatusBar.h"
#include "gui/ClockFrame.h"
#include "gui/MenuFrame.h"

#include "WatchClass.h"

WatchClass *watch;

StatusBar *statusBar;
ClockFrame *clockFrame;
MenuFrame *menuFrame;

Frame *currentFrame;

int frame = 0;
int nextFrame = -1;

void replaceCurrentFrame(int newFrame) {
    currentFrame->unload();

    switch (newFrame) {
        case 0:
            currentFrame = clockFrame;
            break;
        case 1:
            currentFrame = menuFrame;
            break;
    };

    frame = newFrame;
    currentFrame->setup(watch->tft);
}

void switchFrame(int newFrame) {
    nextFrame = newFrame;
}

void handleTouch(point_t p) {
    if (frame == 0) {
        switchFrame(1);
    }
    else if (frame == 1) {
        switchFrame(0);
    }
}

void guiSetup() {
    statusBar = new StatusBar(watch);
    statusBar->setup(watch->tft);

    clockFrame = new ClockFrame(watch, statusBar);

    menuFrame = new MenuFrame(statusBar);

    currentFrame = clockFrame;
    currentFrame->setup(watch->tft);
}

unsigned long _nextDraw = 0;

void guiLoop() {
    if (nextFrame >= 0) {
        replaceCurrentFrame(nextFrame);
        nextFrame = -1;
    }

    const unsigned long t = millis();

    currentFrame->update(t);

    if (t >= _nextDraw) {
        currentFrame->draw();
        _nextDraw = t + 50;
    }
}

void setup() {
    Serial.begin(115200);

    watch = new WatchClass();
    watch->setup();

    watch->touch->onTouch(handleTouch);

    guiSetup();
}

void loop() {
    watch->update();
    if (!watch->isStandby()) {
        guiLoop();
    }
}