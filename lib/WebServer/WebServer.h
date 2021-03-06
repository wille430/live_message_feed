#include "WiFiEsp.h"

#ifndef WebServer_h
#define WebServer_h

class WebServer : public WiFiEspServer
{
private:
    void readRequest(WiFiEspClient client, String *method, String *message);

public:
    void listen(void (*callback)(WiFiEspClient client, String *method, String *message));
    WebServer(uint16_t port) : WiFiEspServer(port){};
    void sendResponse(WiFiEspClient client, String *responseBody);
};

#endif