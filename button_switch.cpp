#include "button_switch.h"

#define MAX_PINS 256

ButtonSwitch* buttonMap[MAX_PINS] = {};

void clickHandler(Button2& button) {
    Switch* sw = buttonMap[button.getPin()];
    if (sw == NULL) {
        return;
    }
    sw->toggle();
}

ButtonSwitch::ButtonSwitch(Button2* button, Switch* sw) : DelegateSwitch(sw), button(button) {
    buttonMap[button->getPin()] = this;
    button->setClickHandler(clickHandler);
    button->setLongClickHandler(clickHandler);
}