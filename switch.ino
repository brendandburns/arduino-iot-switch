#include <Esp.h>
#include <L298N.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "www/generated_html.h"
#include "secret.h"
#include "switch.h"

#define SWITCH_COUNT 2
L298N ctrl1(D1, D2);
L298N ctrl2(D3, D4);
Switch switches[SWITCH_COUNT]{
    Switch(&ctrl1),
    Switch(&ctrl2)};
AsyncWebServer server(80);

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

void handleStatus(AsyncWebServerRequest *req)
{
    String result = "[\n";
    char buffer[64];
    for (int i = 0; i < SWITCH_COUNT; i++) {
        snprintf(buffer, 64, "{ \"ix\": %d, \"on\": %s }", i, switches[i].status() ? "true" : "false");
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

    server.on("/switch", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleStatus(request); });
    server.on("/switch/1/on", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(0, true, request); });
    server.on("/switch/1/off", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(0, false, request); });
    server.on("/switch/2/on", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(1, true, request); });
    server.on("/switch/2/off", HTTP_GET, [](AsyncWebServerRequest *request)
              { handleSwitch(1, false, request); });
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
    delay(5000);
}
