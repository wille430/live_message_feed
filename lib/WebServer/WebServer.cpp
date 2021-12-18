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

        delay(100);

        Serial.println("Stopping client...");
        client.stop();
    }
}

void WebServer::sendResponse(WiFiEspClient client, String *responseBody)
{
    Serial.print("Sending response with body of length ");
    Serial.println(sizeof(responseBody));

    int contentLength = sizeof(responseBody);

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

void WebServer::readRequest(WiFiEspClient client, String *method, char *reqBody)
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
            else
            {
                Serial.println("Encountered a NULL character. Exiting...");
                break;
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
                    Serial.print("Found number in content-type: ");
                    Serial.write(c);
                    Serial.println();

                    contentLength += c;
                }
                else
                {
                    currentLineIsContentLength = false;
                }
            }

            if (request.endsWith("Content-Type: "))
            {
                Serial.print("Found content type!");
                currentLineIsContentLength = true;
            }
        }
    }

    reqBody = new char[contentLength.length()];

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