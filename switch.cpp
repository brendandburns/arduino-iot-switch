#include "switch.h"

Switch::Switch(MotorController* controller) : controller(controller), _on(false) {
    this->off();
}

void Switch::on() { update(true); }
void Switch::off() { update(false); }
bool Switch::status() { return this->_on; }

void Switch::update(bool state) {
    this->_on = state;
    if (state) {
        this->controller->forward();
    } else {
        this->controller->stop();
    }
}
