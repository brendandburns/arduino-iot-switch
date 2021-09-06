#include "switch.h"

#include <Button2.h>


class ButtonSwitch: public DelegateSwitch {
    private:
        Button2* button;
    
    public:
        ButtonSwitch(Button2* button, Switch *sw);
};