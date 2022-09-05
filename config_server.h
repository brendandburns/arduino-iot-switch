#ifndef __CONFIG_SERVER_H__
#define __CONFIG_SERVER_H__

#include <ESPAsyncWebServer.h>

class ConfigServer {
    private:
        AsyncWebServer server;
        String _ssid;
        String _password;
    
    public:
        ConfigServer(int port = 80) : server(port), _ssid(""), _password("") {}
        void begin();
        bool configured();

        const char* ssid();
        const char* password();
    
    protected:
        void handleUpdate(AsyncWebServerRequest *req);
        void handleClear(AsyncWebServerRequest *req);
        void handleIndex(AsyncWebServerRequest *req);
};

#endif