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

String htmlBody = "";

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

void onReqest(WiFiEspClient client, String *method, char *reqBody)
{
  webServer.sendResponse(client, &htmlBody);
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
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
  Serial.println("Listening for requests...");
}

void loop()
{
  delay(10);
  webServer.listen(*onReqest);
}
