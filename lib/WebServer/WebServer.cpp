#include "WebServer.h"
#include "Stream.h"

void WebServer::listen(void (*callback)(WiFiEspClient client, String *method, String *reqBody))
{

    WiFiEspClient client = WiFiEspServer::available();

    if (client)
    {
        Serial.println("New client connected!");

        String method = String("");
        String request = String("");
        WebServer::readRequest(client, &method, &request);

        Serial.println("Calling callback function...");
        callback(client, &method, &request);

        delay(100);

        Serial.println("Stopping client...");
        client.stop();
    }
}

void WebServer::sendResponse(WiFiEspClient client, String *responseBody)
{

    Serial.print("Sending response with body of length ");
    Serial.println((*responseBody).length());

    int contentLength = (*responseBody).length();

    client.print(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n");

    client.print("Content-Length: ");
    client.print(contentLength);
    client.print("\r\n\r\n");

    Serial.println("Sending response body to client...");
    client.print(*responseBody);

    client.println();
}

void WebServer::readRequest(WiFiEspClient client, String *method, String *reqBody)
{
    Serial.println("Reading incoming data...");

    String contentLength = "";
    String currentLine = "";

    String request = "";

    boolean currentLineIsContentLength = false;

    while (client.connected())
    {
        if (client.available())
        {
            char c = client.read();
            Serial.write(c);
            if ((int)c != 0)
            {
                request += c;
            }

            if (request.length() >= 4)
            {
                Serial.println("Last 4 symbols in ASCII code: ");
                Serial.print(request[request.length() - 4]);
                Serial.print(request[request.length() - 3]);
                Serial.print(request[request.length() - 2]);
                Serial.print(request[request.length() - 1]);
                Serial.println();
            }

            if (request.endsWith("\r\n\r\n"))
            {
                break;
            }

            if (request.endsWith("GET"))
            {
                *method = "GET";
            }
            else if (request.endsWith("POST"))
            {
                *method = "POST";
            }

            if (currentLineIsContentLength)
            {
                if (c != '\n' && c != '\r')
                {
                    contentLength += c;
                }
                else
                {
                    currentLineIsContentLength = false;
                }
            }

            if (request.endsWith("Content-Type: "))
            {
                currentLineIsContentLength = true;
            }
        }
    }

    if (*method == "POST")
    {
        Serial.println("Method was POST");
        Serial.print("Expecting body of length: ");
        Serial.println(contentLength.toInt());

        for (int i = 0; i < contentLength.toInt(); i++)
        {
            reqBody += client.read();
        }
    }
}