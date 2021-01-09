#include "TouchModule.h"

TouchModule::TouchModule() {
}

void TouchModule::setup() {
    _ttgo = TTGOClass::getWatch();
    _enabled = true;
}

bool TouchModule::isTouching() {
    return _isTouched;
}

void TouchModule::update(unsigned long t) {
    if (_enabled && t >= _nextUpdate) {
        int16_t x, y;
        if (_onTouch && !_isTouched && _ttgo->getTouch(x, y)) {
            point_t p;
            p.x = x;
            p.y = y;
            _onTouch(p);
        }

        _isTouched = _ttgo->touch->getTouched();
        _nextUpdate = t + _updateDelay;
    }
}

void TouchModule::onTouch(TouchModuleEvent event) {
    _onTouch = event;
}

void TouchModule::enable() {
    _enabled = true;
}

void TouchModule::disable() {
    _enabled = false;
}