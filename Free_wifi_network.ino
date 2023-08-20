#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

const char *ssid = "Free Wifi Network";
const char *pass = "password";
const char chan = 3;

const char *html = R"===(
<!DOCTYPE html>
<html>
<body>
<p>Hello</p>
</body>
</html>
)===";

void defaultPage() {
  webServer.send(200, "text/html", html);
}

void setup() {
   Serial.begin(115200);
   delay(500);
   pinMode(LED_BUILTIN, OUTPUT);
   WiFi.mode(WIFI_AP);
   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
   WiFi.softAP(ssid, pass);
   dnsServer.start(DNS_PORT, "*", apIP);
   webServer.onNotFound(defaultPage);
   webServer.begin();
}

void loop() {
  if (WiFi.softAPgetStationNum() == 0)
  {
    delay(100);
  } else {
    delay(100);
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}
