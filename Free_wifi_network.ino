#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#define PORT 9000

WiFiClient client1;
WiFiServer server(PORT);
IPAddress  apIP(10, 10, 10, 10);

const char *ssid = "Free Wifi Network";
const char *pass = "password";
const char chan = 3;

void setup() {
   Serial.begin(115200);
   delay(500);
   WiFi.mode(WIFI_AP_STA);
   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
   WiFi.softAP(ssid, pass, chan);
   Serial.println("");
   Serial.print("Set up AP: ");
   Serial.println(ssid);
   IPAddress myIP = WiFi.softAPIP();
   Serial.print("Server IP address: ");
   Serial.println(myIP);
   delay(200);
   server.begin();
}

void loop() {
}
