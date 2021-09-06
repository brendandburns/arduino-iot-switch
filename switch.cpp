#include "switch.h"

#include <Arduino.h>

void DelegateSwitch::handleUpdate(bool state, Switch* sw) {
    Serial.println("Handling update!");
    if (!sw) return;
    
    if (state) {
        sw->on();
    } else {
        sw->off();
    }
}


MultiSwitch::MultiSwitch(const char* name, Switch** switches, uint8_t count) : DelegateSwitch(name, NULL), switches(switches), count(count) {
    this->off();
}

void MultiSwitch::update(bool state) {
    for (int i = 0; i < this->count; i++) {
        DelegateSwitch::handleUpdate(state, this->switches[i]);
    }
}

MotorControllerSwitch::MotorControllerSwitch(const char* name, MotorController* controller) : Switch(name), controller(controller) {
    this->off();
}

void MotorControllerSwitch::update(bool state) {
    if (state) {
        this->controller->forward();
    } else {
        this->controller->stop();
    }
}

PinSwitch::PinSwitch(const char* name, uint8_t pin, uint8_t on_value, uint8_t off_value) : Switch(name), pin(pin), on_value(on_value), off_value(off_value) {
    pinMode(this->pin, OUTPUT);
    this->off();
}

void PinSwitch::update(bool state) {
    if (state) {
        digitalWrite(this->pin, this->on_value);
    } else {
        digitalWrite(this->pin, this->off_value);
    }
}

SerialSwitch::SerialSwitch() : Switch("Serial") {}

void SerialSwitch::update(bool state) {
    Serial.println(state ? "ON!" : "OFF!");
}