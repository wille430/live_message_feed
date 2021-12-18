#include "WiFiEsp.h"

#ifndef WebServer_h
#define WebServer_h

class WebServer : public WiFiEspServer
{
private:
    void readRequest(WiFiEspClient client, String *method, char *reqBody);

public:
    void listen(void (*callback)(WiFiEspClient client, String *method, char *reqBody));
    WebServer(uint16_t port) : WiFiEspServer(port){};
    void sendResponse(WiFiEspClient client, String *responseBody);
};

#endif