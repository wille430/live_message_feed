#include "WiFiEsp.h"

#ifndef WebServer_h
#define WebServer_h

class WebServer
{

private:
    WiFiEspServer *server;

public:
    awaitHtmlBody()
    {
        WiFiEspClient client = (*server).available();

        while (!client)
        {
            client = (*server).available();

            while (client.connected())
            {
                if (client.available())
                {
                    readRequest(client);
                }
            }

            delay(10);
        }
    }

    WebServer(WiFiEspServer &server)
    {
        server = server;
    }

    void listen()
    {
        WiFiEspClient client = (*server).available();

        if (client)
        {

            boolean currentLineIsBlank = true;

            while (client.connected())
            {
                if (client.available())
                {
                    readReqest(client, &currentLineIsBlank);
                }
            }

            client.stop();
            Serial.println("Client disconnected");
        }
    }

    void sendResponseHeader(WiFiEspClient *client)
    {
        (*client).print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n");
    }

    void readReqest(WiFiEspClient client, boolean *currentLineIsBlank)
    {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank)
        {
            break;
        }

        if (c == '\n')
        {
            *currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
            *currentLineIsBlank = false;
        }
    }

    // void sendHtmlFile(char path[])
    // {
    //     FILE *file;
    //     file = fopen(path, "r");
    // }
};

#endif