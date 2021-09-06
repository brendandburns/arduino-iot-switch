#include "button_switch.h"

#define MAX_PINS 256

ButtonSwitch* buttonMap[MAX_PINS] = {};

void clickHandler(Button2& button) {
    Switch* sw = buttonMap[button.getAttachPin()];
    if (sw == NULL) {
        return;
    }
    sw->toggle();
}

ButtonSwitch::ButtonSwitch(const char* name, Button2* button, Switch* sw) : DelegateSwitch(name, sw), button(button) {
    buttonMap[button->getAttachPin()] = this;
    button->setClickHandler(clickHandler);
    button->setLongClickHandler(clickHandler);
    this->off();
}