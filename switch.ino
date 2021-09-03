#include <Esp.h>
#include <L298N.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <EEPROM.h>

#include <Button2.h>

#include "www/generated_html.h"
#include "secret.h"
#include "switch.h"

#define SWITCH_COUNT 3
L298N ctrl1(D1, D2);
L298N ctrl2(D3, D4);
Switch *switches[SWITCH_COUNT]{
    new MotorControllerSwitch("Fountain", &ctrl1),
    new MotorControllerSwitch("Light", &ctrl2),
    new SerialSwitch()};
AsyncWebServer server(80);

#define BUTTON_PIN 10

Button2 button(BUTTON_PIN);

void writeSwitchState()
{
    EEPROM.begin(SWITCH_COUNT);
    for (int i = 0; i < SWITCH_COUNT; i++)
    {
        EEPROM.write(i, switches[i]->status() ? 1 : 0);
    }
    EEPROM.commit();
    EEPROM.end();
}

void readSwitchState()
{
    EEPROM.begin(SWITCH_COUNT);
    for (int i = 0; i < SWITCH_COUNT; i++)
    {
        uint8_t value = EEPROM.read(i);
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

void flipSwitch(Switch *sw, bool on)
{
    if (on)
    {
        sw->on();
    }
    else
    {
        sw->off();
    }
    writeSwitchState();
}

void handleSwitch(int ix, bool on, AsyncWebServerRequest *req)
{
    flipSwitch(switches[ix], on);
    if (on)
    {
        req->send(200, "text/plain", String("Switch ") + ix + " is on");
    }
    else
    {
        req->send(200, "text/plain", String("Switch ") + ix + " is off");
    }
}

void handleStatus(AsyncWebServerRequest *req)
{
    String result = "[\n";
    char buffer[128];
    for (int i = 0; i < SWITCH_COUNT; i++)
    {
        Switch *sw = switches[i];
        snprintf(buffer, 64,
                 "{ \"ix\": %d, \"on\": %s, \"name\": \"%s\" }",
                 i,
                 sw->status() ? "true" : "false",
                 sw->name());
        result = result + buffer + ((i + 1 < SWITCH_COUNT) ? ",\n" : "\n");
    }
    result += "]\n";

    req->send(200, "application/json", result.c_str());
}

void setup()
{
    Serial.begin(9600);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());

    readSwitchState();

    for (int i = 0; i < SWITCH_COUNT; i++)
    {
        char buffer[32];
        snprintf(buffer, 32, "/switch/%d/on", i);
        server.on(buffer, HTTP_GET, [i](AsyncWebServerRequest *request)
                  { handleSwitch(i, true, request); });
        snprintf(buffer, 32, "/switch/%d/off", i);
        server.on(buffer, HTTP_GET, [i](AsyncWebServerRequest *request)
                  { handleSwitch(i, false, request); });
    }

    server.on("/switch", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleStatus(request); });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/javascript", script_js); });
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", index_html); });
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", index_html); });

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/html", "<h1>Not found</h1>"); });

    server.begin();

    button.setClickHandler([](Button2& btn) { switches[0]->toggle(); });
}

void loop()
{
    delay(5000);
}
