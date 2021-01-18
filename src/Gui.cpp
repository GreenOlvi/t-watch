#include "Gui.h"

Gui::Gui(WatchClass *watch)
    : _watch(watch), _tft(_watch->tft) {
}

void Gui::setup() {
    _statusBar = new StatusBar(_watch);
    _statusBar->setup(_tft);

    _clockFrame = new ClockFrame(this, _watch, _statusBar);

    _menuFrame = new MenuFrame(this, _watch, _statusBar);

    _currentFrame = _clockFrame;
    _currentFrame->setup(_tft);
}

void Gui::update(const unsigned long t) {
    if (_watch->isStandby())
        return;

    if (_nextFrame >= 0)
    {
        replaceCurrentFrame(_nextFrame);
        _nextFrame = -1;
    }

    _currentFrame->update(t);

    if (t >= _nextDraw) {
        _currentFrame->draw();
        _nextDraw = t + 50;
    }
}

void Gui::replaceCurrentFrame(int newFrame) {
    _currentFrame->unload();

    switch (newFrame) {
        case 0:
            _currentFrame = _clockFrame;
            break;
        case 1:
            _currentFrame = _menuFrame;
            break;
    };

    _frame = newFrame;
    _currentFrame->setup(_tft);
}

void Gui::switchFrame(int newFrame) {
    _nextFrame = newFrame;
}

void Gui::handleTouch(point_t p) {
    _currentFrame->onTouch(p);
}
