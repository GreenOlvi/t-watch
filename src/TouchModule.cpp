#include "TouchModule.h"

TouchModule::TouchModule() {
}

void TouchModule::setup() {
    _ttgo = TTGOClass::getWatch();
}

bool TouchModule::isTouching() {
    return _isTouched;
}

void TouchModule::update(unsigned long t) {
    if (t >= _nextUpdate) {
        int16_t x, y;
        if (_onTouch && !_isTouched && _ttgo->getTouch(x, y)) {
            _onTouch(x, y);
        }

        _isTouched = _ttgo->touch->getTouched();
        _nextUpdate = t + _updateDelay;
    }
}

void TouchModule::onTouch(TouchModuleEvent event) {
    _onTouch = event;
}