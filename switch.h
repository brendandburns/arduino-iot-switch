#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <MotorController.h>

class Switch {
    private:
        MotorController* controller;
        bool _on;
    
    public:
        Switch(MotorController* controller);

        void on();
        void off();
        bool status();
    
    private:
        void update(bool state);
};

#endif // __SWITCH_H__