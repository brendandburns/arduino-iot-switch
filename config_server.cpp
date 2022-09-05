#include "config_server.h"

#include "wifi_handler.h"
#include "www/config_html.h"

void ConfigServer::handleUpdate(AsyncWebServerRequest *req)
{
    if(req->hasParam("ssid"))
    {
        AsyncWebParameter* p = req->getParam("ssid");
        this->_ssid = p->value().c_str();
    }
    if(req->hasParam("password"))
    {
        this->_password = req->getParam("password")->value().c_str();
    }
    Serial.println(this->_ssid + " : " + this->_password);
    req->send(200, "text/html", "<h1>ok</h1>");
}

void ConfigServer::handleClear(AsyncWebServerRequest *req)
{
    bool ok = WifiHandler::clearConfig();
    if (ok) {
        req->send(200, "text/html", "<h1>ok</h1>");
        ESP.restart();
    } else {
        req->send(500, "text/html", "<h1>Failed to clear config</h1>");
    }
}

void ConfigServer::handleIndex(AsyncWebServerRequest *req)
{
    req->send(200, "text/html", config_index_html);
}

void ConfigServer::begin()
{
    this->server.on("/update", HTTP_GET, [this](AsyncWebServerRequest *request)
        { this->handleUpdate(request); });
    
    this->server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request)
        { this->handleClear(request); });
    
    this->server.on("/index.html", HTTP_GET, [this](AsyncWebServerRequest *request)
        { this->handleIndex(request); });
    
    this->server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
        { this->handleIndex(request); });

    this->server.onNotFound([](AsyncWebServerRequest *request)
        { request->send(404, "text/html", "<h1>Not found</h1>"); });

    this->server.begin();
}

bool ConfigServer::configured() {
    return this->_ssid.length() > 0 && this->_password.length() > 0;
}

const char* ConfigServer::ssid() {
    return this->_ssid.c_str();
}

const char* ConfigServer::password() {
    return this->_password.c_str();
}