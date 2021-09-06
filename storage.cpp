#include "storage.h"

#include <HardwareSerial.h>
#include <EEPROM.h>

void writeSwitchState(Switch** switches, size_t count)
{
    EEPROM.begin(count);
    for (int i = 0; i < count; i++)
    {
        Serial.println(String("Storing ") + switches[i]->status() + " for " + switches[i]->name());
        EEPROM.write(i, switches[i]->status() ? 1 : 0);
    }
    EEPROM.commit();
    EEPROM.end();
}

void readSwitchState(Switch** switches, size_t count)
{
    EEPROM.begin(count);
    for (int i = 0; i < count; i++)
    {
        uint8_t value = EEPROM.read(i);
        Serial.println(String("Reading ") + switches[i]->status() + " for " + switches[i]->name());
        if (value == 1)
        {
            switches[i]->on();
        }
        else
        {
            switches[i]->off();
        }
    }
}
