#include "WebServer.h"
#include "Stream.h"

void WebServer::listen(void (*callback)(WiFiEspClient client, String *method, char *reqBody))
{

    WiFiEspClient client = WiFiEspServer::available();

    if (client)
    {
        Serial.println("New client connected!");

        String method = String("");
        char *reqBody;
        WebServer::readRequest(client, &method, reqBody);

        Serial.println("Calling callback function...");
        callback(client, &method, reqBody);

        Serial.println("Stopping client...");
        client.stop();

        delay(20);
    }
}

void WebServer::sendResponse(WiFiEspClient client, String *responseBody)
{
    Serial.println("Sending response...");

    client.print(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n");

    delay(20);

}

void WebServer::readRequest(WiFiEspClient client, String *method, char *reqBody)
{
    Serial.println("Reading incoming data...");

    String message = "";

    while (client.connected())
    {
        if (client.available())
        {
            String line = client.readStringUntil('/');

            message = client.readStringUntil(' ');

            Serial.println("Message:");
            Serial.println(message);

            break;
        }

        delay(10);
    }
}