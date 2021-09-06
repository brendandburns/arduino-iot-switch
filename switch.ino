#include <Esp.h>
#include <L298N.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <EEPROM.h>

#include <Button2.h>

#include "button_switch.h"
#include "persistent_switch.h"
#include "secret.h"
#include "storage.h"
#include "switch.h"
#include "www/generated_html.h"

#define BUTTON_PIN D5
Button2 button(BUTTON_PIN);

#define SWITCH_COUNT 3
L298N ctrl1(D1, D2);
L298N ctrl2(D3, D4);

Switch *switches[SWITCH_COUNT]{
    new PersistentSwitch(new MotorControllerSwitch("Fountain", &ctrl1), switches, SWITCH_COUNT),
    new PersistentSwitch(new MotorControllerSwitch("Light", &ctrl2), switches, SWITCH_COUNT),
    new ButtonSwitch(&button, new SerialSwitch())};

AsyncWebServer server(80);

void handleSwitch(int ix, bool on, AsyncWebServerRequest *req)
{
    if (on)
    {
        switches[ix]->on();
        req->send(200, "text/plain", String("Switch ") + ix + " is on");
    }
    else
    {
        switches[ix]->off();
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

    readSwitchState(switches, SWITCH_COUNT);
    PersistentSwitch::begin();

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
}

void loop()
{
    button.loop();
}
