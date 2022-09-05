#ifndef __WIFI_HANDLER_H__
#define __WIFI_HANDLER_H__

#include <Arduino.h>

#include "config_server.h"

class WifiHandler {
    private:
        String ssid;
        String pass;
        ConfigServer *config;

        void writeString(uint8_t ix, const char* str);
        void readString(uint8_t ix, char* buff, uint8_t len);

    public:
        WifiHandler(const char* ssid, const char* pass);
    
        // Connects to the SSID using password waits
        // until connected.
        void connect();

        // Sets up a hotspot and waits for configuration.
        void waitForConfiguration();

        // Attempts to connect to SSID if present, otherwise
        // Sets up a hotspot and waits for configuration.
        void connectOrWaitForConfiguration();

        bool writeConfig();
        bool readConfig();

        static bool clearConfig();
};

#endif // __WIFI_HANDLER_H__