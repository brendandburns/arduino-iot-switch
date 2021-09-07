#ifndef __SERVER_H__
#define __SERVER_H__

#include "switch.h"

#include <ESPAsyncWebServer.h>

class SwitchServer {
    private:
        Switch** switches;
        size_t count;
        AsyncWebServer server;
    
    public:
        SwitchServer(Switch **switches, size_t count, int port) : switches(switches), count(count), server(port) {}
        void begin();
    
    protected:
        void handleSwitch(int ix, bool on, AsyncWebServerRequest *req);
        void handleStatus(AsyncWebServerRequest *req);
};

#endif