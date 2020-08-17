#include "MotorModule.h"

MotorModule::MotorModule(int pin) : _pin(pin), _channel(1), _frequency(12000) {
}

MotorModule::~MotorModule() {
    ledcDetachPin(_pin);
    delete _ticker;
}

void MotorModule::setup() {
    ledcAttachPin(_pin, _channel);
    ledcSetup(_channel, _frequency, 8);
    ledcWrite(_channel, 0);
    _ticker = new Ticker;
}

void MotorModule::vibe(unsigned int t) {
    ledcWriteTone(_channel, _frequency);
    _ticker->once_ms(t, +[](const uint8_t channel) {
        ledcWriteTone(channel, 0);
    }, _channel);
}

void _turnOffMotor(const uint8_t channel) {
    ledcWriteTone(channel, 0);
}

void MotorModule::shortVibe() {
    vibe(10);
}