#include "WebServer.h"
#include "Stream.h"

void WebServer::listen(void (*callback)(WiFiEspClient client, String *method, String *reqBody))
{

    WiFiEspClient client = WiFiEspServer::available();

    if (client)
    {
        Serial.println("New client connected!");

        String method = String("");
        String message = "";
        WebServer::readRequest(client, &method, &message);

        Serial.println("Calling callback function...");
        callback(client, &method, &message);

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

void WebServer::readRequest(WiFiEspClient client, String *method, String *message)
{
    Serial.println("Reading incoming data...");

    while (client.connected())
    {
        if (client.available())
        {
            String line = client.readStringUntil('/');

            *message = client.readStringUntil(' ');

            Serial.println("Message:");
            Serial.println(*message);

            break;
        }

        delay(10);
    }
}