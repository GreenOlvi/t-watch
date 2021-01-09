#include "PowerModule.h"

PowerModule::PowerModule(AXP20X_Class *power)
    : _power(power) {
}

void PowerModule::setup() {
    _instance = this;

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, InterruptHandler, FALLING);

    _power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    _power->clearIRQ();
}

void PowerModule::update(const unsigned long t) {
    if (_irq) {
        _power->readIRQ();

        if (_power->isPEKShortPressIRQ() && _hasOnButtonShortPress) {
            _onButtonShortPressHandler();
        }

        _power->clearIRQ();
        _irq = false;
    }
}

void PowerModule::onButtonShortPress(PowerModuleEvent handler) {
    _onButtonShortPressHandler = handler;
    _hasOnButtonShortPress = true;
}

// only way to attach interrupt to pin is by using static method, hence the ugly singleton hack
PowerModule *PowerModule::_instance;

void PowerModule::InterruptHandler() {
    if (_instance) {
        _instance->_irq = true;
    }
}