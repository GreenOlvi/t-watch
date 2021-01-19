#include "PowerModule.h"

PowerModule::PowerModule(AXP20X_Class *power)
    : _power(power) {
}

void PowerModule::setup() {
    _instance = this;

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, InterruptHandler, FALLING);

    _power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, true);
    _power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_CHARGING_IRQ | AXP202_CHARGING_FINISHED_IRQ, true);
    _power->clearIRQ();

    _power->setPowerOutPut(AXP202_EXTEN, false);
    _power->setPowerOutPut(AXP202_DCDC2, false);
    _power->setPowerOutPut(AXP202_LDO3, false);
    _power->setPowerOutPut(AXP202_LDO4, false);
}

void PowerModule::update(const unsigned long t) {
    if (_irq) {
        _power->readIRQ();

        if (_power->isPEKShortPressIRQ() && _hasOnButtonShortPress) {
            _onButtonShortPressHandler();
        }

        if (_power->isChargingIRQ()) {
            _isCharging = true;
        }

        if (_power->isChargingDoneIRQ()) {
            _isCharging = false;
        }

        if (_power->isVbusRemoveIRQ()) {
            _isCharging = false;
        }

        _power->clearIRQ();
        _irq = false;
    }
}

void PowerModule::onButtonShortPress(PowerModuleEvent handler) {
    _onButtonShortPressHandler = handler;
    _hasOnButtonShortPress = true;
}

bool PowerModule::isCharging() {
    return _isCharging;
}

float PowerModule::getBatteryVoltage() {
    return _power->getBattVoltage() / 1000.0;
}

int PowerModule::getBatteryPercentage() {
    return _power->getBattPercentage();
}

// only way to attach interrupt to pin is by using static method, hence the ugly singleton hack
PowerModule *PowerModule::_instance;

void PowerModule::InterruptHandler() {
    if (_instance) {
        _instance->_irq = true;
    }
}