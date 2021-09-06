#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <stdint.h>
#include <WString.h>
#include <MotorController.h>

class Switch {
    private:
        bool _on;
        String _name;
    
    public:
        Switch(const char* name) : _on(false), _name(name) {}

        void on() { _on = true; update(true); }
        void off() { _on = false; update(false); }
        void toggle() { _on = !_on; update(_on); }
        bool status() { return _on; }
        const char* name() { return _name.c_str(); }
    
    protected:
        virtual void update(bool state) = 0;
};

class DelegateSwitch : public Switch {
    private:
        Switch* delegate;
    
    public:
        DelegateSwitch(Switch* delegate) : Switch(delegate->name()), delegate(delegate) {}
    
    protected:
        virtual void update(bool state) { DelegateSwitch::handleUpdate(state, delegate); }
    
    public:
        static void handleUpdate(bool state, Switch* sw);
};

class MultiSwitch : public Switch {
    private:
        uint8_t count;
        Switch** switches;
    
    public:
        MultiSwitch(const char* name, Switch* switches[], uint8_t count);
    
    protected:
        virtual void update(bool state);
};

class MotorControllerSwitch : public Switch {
    private:
        MotorController* controller;
    
    public:
        MotorControllerSwitch(const char* name, MotorController* controller);
    
    protected:
        virtual void update(bool state);
};

class PinSwitch : public Switch {
    private:
        uint8_t pin;
        uint8_t on_value;
        uint8_t off_value;
        
    public:
        PinSwitch(const char* name, uint8_t pin, uint8_t on_value, uint8_t off_value);

    protected:
        virtual void update(bool state);
};

class SerialSwitch : public Switch {
    public:
        SerialSwitch();
    protected:
        virtual void update(bool state);
};

#endif // __SWITCH_H__