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
        if (!_isTouched && _ttgo->touch->touched() && _onTouch) {
            TP_Point point = _ttgo->touch->getPoint();
            _onTouch(point);
        }

        _isTouched = _ttgo->touch->touched();
        _nextUpdate = t + _updateDelay;
    }
}

void TouchModule::onTouch(TouchModuleEvent event) {
    _onTouch = event;
}