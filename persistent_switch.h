#ifndef __PERSISTENT_SWITCH_H__
#define __PERSISTENT_SWITCH_H__

#include "switch.h"

class PersistentSwitch : public DelegateSwitch {
    private:
        Switch** switches;
        size_t count;
    
    public:
        PersistentSwitch(Switch* sw, Switch** switches, size_t count) : DelegateSwitch(sw), switches(switches), count(count) {}
    
    private:
        virtual void update(bool state);

    public:
        static void begin(Switch** switches, size_t count);
};

#endif