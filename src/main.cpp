#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFiEsp.h"
#include "WebServer.h"
#include <secrets.h>
#include "MessageFeed.h"

// display library
#include "U8glib.h"

// skapa display
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

// TX, RX pins för wifi modul
SoftwareSerial EspSerial(6, 7);

// Starta webserver på port 80
WebServer webServer(80);

// Nätverksnamn och lösenord
char ssid[] = SECRET_SSID;
char password[] = SECRET_PASSWORD;

int status = WL_IDLE_STATUS;

String htmlBody = "";
MessageFeed messageFeed;

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

void onReqest(WiFiEspClient client, String *method, String *message)
{
  webServer.sendResponse(client, &htmlBody);
  messageFeed.push((*message + '\n').c_str());
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

void draw_screen()
{
  oled.setFont(u8g_font_helvB08);
  int font_size = 12;

  for (int i = 0; i < messageFeed.total_messages; i++) {
    // beräkna y-värde
    int line_height = font_size * (i + 1);

    // skriv meddelandet på skärmen
    oled.drawStr(0, line_height, messageFeed.feed[i].c_str());
  }
}

void loop()
{
  delay(10);
  webServer.listen(*onReqest);

  oled.firstPage();
  do
  {
    draw_screen();
  } while (oled.nextPage());
}
