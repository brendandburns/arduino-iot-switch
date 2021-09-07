#include "persistent_switch.h"

#include "storage.h"

bool begun = false;

void PersistentSwitch::update(bool state) {
    DelegateSwitch::update(state);
    if (begun) {
        writeSwitchState(this->switches, this->count);
    }
}

void PersistentSwitch::begin(Switch **switches, size_t count) {
    readSwitchState(switches, count);
    begun = true;
}