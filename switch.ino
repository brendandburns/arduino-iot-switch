
#include <Esp.h>
#include <L298N.h>

#include <ESP8266WiFi.h>

#include <Button2.h>

#include "button_switch.h"
#include "persistent_switch.h"
#include "secret.h"
#include "server.h"
#include "switch.h"
#include "wifi_handler.h"
#include "config_server.h"

#define BUTTON_PIN D5
Button2 button(BUTTON_PIN);

#define SWITCH_COUNT 3
L298N ctrl1(D1, D2);
L298N ctrl2(D3, D4);

Switch *switches[SWITCH_COUNT]{
    new ButtonSwitch(
        &button,
        new MultiSwitch("Fountain", new Switch*[2] {
            new PersistentSwitch(new MotorControllerSwitch("Fountain", &ctrl1), switches, SWITCH_COUNT),
            new PinSwitch("Ring", D6, HIGH, LOW)
        }, 2)),
    new PersistentSwitch(new MotorControllerSwitch("Light", &ctrl2), switches, SWITCH_COUNT),
    new SerialSwitch("Test")};

SwitchServer server(switches, SWITCH_COUNT, 80);
WifiHandler handler("name", "pass");

void setup()
{
    Serial.begin(9600);

    handler.connectOrWaitForConfiguration();

    PersistentSwitch::begin(switches, SWITCH_COUNT);
    server.begin();
}

void loop()
{
    button.loop();
}
