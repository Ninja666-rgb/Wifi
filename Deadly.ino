#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String the_html = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<title>Don't click</title>\n"
                  "</head>\n"
                  "<body>\n"
                  "<p>DON'T CLICK THE BIG RED BUTTON</p>\n"
                  "<script>function funnystuff(){\n"
                  "  alert('WHY!');\n"
                  "  alert('YOU STUPID LITTLE HUMAN BEING!');\n"
                  "  alert('I DIDN\\'T WANT TO DO THIS...');\n"
                  "  alert('(Boss music starts)');\n"
                  "  alert('BUT NOW YOU MADE ME!');\n"
                  "  alert('PREPARE TO DIE!');\n"
                  "  for(let i = 0; i > -1; i*=2){\n"
                  "    location.reload();\n"
                  "  }\n"
                  "}\n"
                  "</script><button style=\"background-color:red; border:none; color:white; font-size:20px;\" onclick=\"funnystuff()\">DON'T PRESS</button>\n"
                  "</body>\n"
                  "</html>\n";
// Handle Root
void defaultPage() {
  webServer.send(200, "text/html", the_html);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Do not use this wifi");
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
