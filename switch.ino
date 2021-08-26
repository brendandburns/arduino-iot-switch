#include <Esp.h>
#include <L298N.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "secret.h"

L298N switch1(D1, D2);
L298N switch2(D3, D4);
AsyncWebServer server(8080);

void switchOne(bool on) {
    if (on) {
        switch1.forward();
    } else {
        switch1.stop();
    }
}

void switchTwo(bool on) {
    if (on) {
        switch2.forward();
    } else {
        switch2.stop();
    }
}

bool sw1_on = false;
bool sw2_on = false;

void setup()
{
    Serial.begin(9600);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());

    server.on("/switch/1/on", HTTP_GET, [](AsyncWebServerRequest *request){
        sw1_on = true;
        switchOne(sw1_on);
        request->send(200, "text/html", "<h1>Switch One On</h1>");
    });

    server.on("/switch/1/off", HTTP_GET, [](AsyncWebServerRequest *request){
        sw1_on = false;
        switchOne(sw1_on);
        request->send(200, "text/html", "<h1>Switch One Off</h1>");
    });

    server.on("/switch/2/on", HTTP_GET, [](AsyncWebServerRequest *request){
        sw2_on = true;
        switchTwo(sw1_on);
        request->send(200, "text/html", "<h1>Switch Two On</h1>");
    });

    server.on("/switch/2/off", HTTP_GET, [](AsyncWebServerRequest *request){
        sw2_on = false;
        switchTwo(sw2_on);
        request->send(200, "text/html", "<h1>Switch Two Off</h1>");
    });

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/html", "<h1>Not found</h1>");
    });

    server.begin();
}

void loop()
{
    delay(5000);
}
