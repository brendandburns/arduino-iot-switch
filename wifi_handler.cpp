#include "wifi_handler.h"

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include "config_server.h"

WifiHandler::WifiHandler(const char* ssid, const char* pass)
    : ssid(ssid), pass(pass), config(NULL) {}

void WifiHandler::connect() {
    WiFi.begin(this->ssid, this->pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());

    this->config = new ConfigServer(8080);
    this->config->begin();
}

void WifiHandler::waitForConfiguration() {
    Serial.println("Setting configuration");
    String apSsid("esp");
    apSsid += ESP.getChipId();
    Serial.println("Starting SSID: " + apSsid);
    WiFi.softAP(apSsid.c_str(), "password");

    IPAddress IP = WiFi.softAPIP();
    Serial.print("Serving at: http://");
    Serial.print(IP);
    Serial.println("/update");

    ConfigServer configServer;
    configServer.begin();

    while (!configServer.configured()) {
        delay(1000);
    }

    this->ssid = configServer.ssid();
    this->pass = configServer.password();
    
    if (!this->writeConfig()) {
        Serial.println("Failed to write configuration!");
    } else {
        Serial.println("Configuration written");
    }

    Serial.println("Shutting down wifi...");
    WiFi.mode(WIFI_OFF);

    delay(5000);

    Serial.println("Rebooting");
    ESP.restart();
}

void WifiHandler::connectOrWaitForConfiguration() {
    this->readConfig();
    bool found = false;
    int numberOfNetworks = WiFi.scanNetworks();
    for(int i =0; i<numberOfNetworks; i++){
 
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.println("-----------------------");

      found |= WiFi.SSID(i).equals(ssid); 
    }
    if (!found) {
        this->waitForConfiguration();
    }
    this->connect();
}

bool WifiHandler::readConfig() {
    EEPROM.begin(256);
    uint8_t val = EEPROM.read(0);
    char buffer[128];
    if (val != 254) {
        Serial.print("No configuration: ");
        Serial.println(val);
        return false;
    }
    this->readString(1, buffer, 128);
    this->ssid = buffer;
    this->readString(1 + this->ssid.length() + 1, buffer, 128);
    this->pass = buffer;

    Serial.println(String("Read: ") + this->ssid + " : " + this->pass);
    return true;
}

bool WifiHandler::writeConfig() {
    EEPROM.begin(256);
    EEPROM.write(0, (uint8_t)254);
    this->writeString(1, this->ssid.c_str());
    this->writeString(this->ssid.length() + 2, this->pass.c_str());
    return EEPROM.commit();
}

void WifiHandler::writeString(uint8_t ix, const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        EEPROM.write(ix + i, str[i]);
    }
    EEPROM.write(ix + len, 0);
}

void WifiHandler::readString(uint8_t ix, char* buff, uint8_t len) {
    for (int i = 0; i < len; i++) {
        buff[i] = EEPROM.read(ix + i);
        if (buff[i] == 0) {
            break;
        }
    }
}

bool WifiHandler::clearConfig() {
    EEPROM.begin(1);
    EEPROM.write(0, 0);
    return EEPROM.commit();
}
