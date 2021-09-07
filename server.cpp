#include "server.h"
#include "www/generated_html.h"

void SwitchServer::handleSwitch(int ix, bool on, AsyncWebServerRequest *req)
{
    if (on)
    {
        this->switches[ix]->on();
        req->send(200, "text/plain", String("Switch ") + ix + " is on");
    }
    else
    {
        this->switches[ix]->off();
        req->send(200, "text/plain", String("Switch ") + ix + " is off");
    }
}

void SwitchServer::handleStatus(AsyncWebServerRequest *req)
{
    String result = "[\n";
    char buffer[128];
    for (int i = 0; i < count; i++)
    {
        Switch *sw = switches[i];
        snprintf(buffer, 64,
                 "{ \"ix\": %d, \"on\": %s, \"name\": \"%s\" }",
                 i,
                 sw->status() ? "true" : "false",
                 sw->name());
        result = result + buffer + ((i + 1 < count) ? ",\n" : "\n");
    }
    result += "]\n";

    req->send(200, "application/json", result.c_str());
}

void SwitchServer::begin()
{
    for (int i = 0; i < this->count; i++)
    {
        char buffer[32];
        snprintf(buffer, 32, "/switch/%d/on", i);
        server.on(buffer, HTTP_GET, [this, i](AsyncWebServerRequest *request)
                  { this->handleSwitch(i, true, request); });
        snprintf(buffer, 32, "/switch/%d/off", i);
        server.on(buffer, HTTP_GET, [this, i](AsyncWebServerRequest *request)
                  { this->handleSwitch(i, false, request); });
    }

    server.on("/switch", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleStatus(request); });

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