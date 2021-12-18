#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFiEsp.h"
#include "WebServer.h"
#include <secrets.h>

SoftwareSerial EspSerial(6, 7);

// Starta webserver på port 80
WebServer webServer(80);

// Nätverksnamn och lösenord
char ssid[] = SECRET_SSID;
char password[] = SECRET_PASSWORD;

int status = WL_IDLE_STATUS;

String htmlBody = String("");

void printWifiStatus()
{

  // Print the SSID of the network
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void onReqest(WiFiEspClient client, String *method, String *request)
{

  RingBuffer emptyBuffer(8);
  emptyBuffer.init();

  if ((*method).equals("GET"))
  {
    Serial.println("Sending html from buffer...");
    Serial.println("---------- HTML body ----------");
    Serial.println(htmlBody);
    webServer.sendResponse(client, &htmlBody);
  }
  else if ((*method).equals("POST"))
  {
    Serial.println("Storing POST request body inside ring buffer...");
    htmlBody = *request;

    Serial.println("Stored POST req body inside ring buffer.");
    Serial.print("POST req length: ");
    Serial.println((*request).length());

    webServer.sendResponse(client, &htmlBody);
  }
  else
  {
    Serial.print(*method);
    Serial.println(" method is not supported. Sending default empty response...");
    webServer.sendResponse(client, &htmlBody);
  }
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  EspSerial.begin(9600);

  // Initiera wifi modulen
  WiFi.init(&EspSerial);

  // Fortsätt inte om det inte finns någon wifi shield
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.print("WiFi shield not present");

    while (true)
      ;
  }

  // Försök att ansluta till wifi:t
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, password);
  }

  Serial.println("Successfully connected to wifi!");
  printWifiStatus();

  webServer.begin();
}

void loop()
{
  webServer.listen(*onReqest);
}
