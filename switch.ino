#include <Esp.h>
#include <L298N.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "secret.h"
#include "switch.h"

#define SWITCH_COUNT 2
L298N ctrl1(D1, D2);
L298N ctrl2(D3, D4);
Switch switches[SWITCH_COUNT]{
    Switch(&ctrl1),
    Switch(&ctrl2)};
AsyncWebServer server(8080);

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
}

void handleSwitch(int ix, bool on, AsyncWebServerRequest *req)
{
    flipSwitch(&(switches[ix]), on);
    if (on) {
        req->send(200, "text/plain", String("Switch ") + ix + " is on");
    } else {
        req->send(200, "text/plain", String("Switch ") + ix + " is off");
    }
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

    server.on("/switch/1/on", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(0, true, request); });
    server.on("/switch/1/off", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(0, false, request); });
    server.on("/switch/2/on", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(1, true, request); });
    server.on("/switch/2/off", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(1, false, request); });

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/html", "<h1>Not found</h1>"); });

    server.begin();
}

void loop()
{
    delay(5000);
}
