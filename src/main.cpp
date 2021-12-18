#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFiEsp.h"
#include "WebServer.h"
#include <secrets.h>

SoftwareSerial EspSerial(6, 7);

// Starta webserver på port 80
WebServer webServer(80);
// Start client
WiFiEspClient client;

// Nätverksnamn och lösenord
char ssid[] = SECRET_SSID;
char password[] = SECRET_PASSWORD;

int status = WL_IDLE_STATUS;

String htmlBody;

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

void fetchHTMLBody()
{
  if (client.connect("raw.githubusercontent.com", 80))
  {
    Serial.println("Connected to the server. Making request...");

    client.print(
        "GET /wille430/live_message_feed/master/lib/sendFile/public/index.html HTTP/1.1\r\n"
        "Host: raw.githubusercontent.com\r\n"
        "user-agent: curl/7.80.0\r\n"
        "accept: */*\r\n"
        "\r\n");

    Serial.println("Reading request file...");

    boolean isBody = false;
    String header = "";
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);

      if (!isBody)
      {
        header += c;
      }
      else
      {
        Serial.print("Reading response body...");
        htmlBody = client.readStringUntil('\0');
      }

      if (header.endsWith("\r\n\r\n"))
      {
        Serial.println("Found end of header");
        isBody = true;
      }
    }

    client.stop();
    Serial.println("Text content was succesfully saved!");
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
  fetchHTMLBody();
}

void loop()
{
  webServer.listen(*onReqest);
}
