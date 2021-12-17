#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFiEsp.h"
#include "../lib/WebServer.h"

SoftwareSerial EspSerial(6, 7);

// Starta webserver på port 80
WiFiEspServer server(80);
WebServer webServer(server);

// Nätverksnamn och lösenord
char ssid[] = "NTIG Guest";
char password[] = "TeknikPassion";

int status = WL_IDLE_STATUS;

char serverIp[] = "ws://172.104.148.23";
int port = 8080;

WiFiEspClient client;

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

  // if (client.connect(serverIp, port))
  // {
  //   Serial.println("Connected to server");
  //   client.println("");
  // }

  // Starta webservern
  server.begin();
}

void loop()
{
  // if (!client.connected())
  // {
  //   Serial.print("Disconnecting from server...");
  //   client.stop();

  //   while (true)
  //     ;
  // }
  webServer.awaitHtmlBody();
  webServer.listen();
}