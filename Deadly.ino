#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String the_html = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<title>Don't click</title>\n"
                  "</head>\n"
                  "<body>\n"
                  "<p>DON'T CLICK THE BIG RED BUTTON</p>\n"
                  "<button style=\"background-color:red; border:none; color:white; font-size:20px;\" onclick=\"window.location = '/regret'\">DON'T PRESS</button>\n"
                  "</body>\n"
                  "</html>\n";

String the_html2 = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<title>Regret</title>\n"
                  "</head>\n"
                  "<body>\n"
                  "<p id = 'reg'>No regert?</p>\n"
                  "<script>\n"
                  "  for(let i = 0; i < 1000; i++){\n"
                  "    document.getElementById('reg').innerHTML = document.getElementById('reg').innerText+'<br>No regert?';\n"
                  "  }\n"
                  "  for(let i = 0; i > -1; i*=2){\n"
                  "    location.reload();\n"
                  "  }\n"
                  "</script>\n"
                  "</body>\n"
                  "</html>\n";

// Handle Root
void defaultPage() {
  webServer.send(200, "text/html", the_html);
}

void regret() {
  webServer.send(200, "text/html", the_html2);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Do not use this wifi");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound(defaultPage);
  webServer.on("/regret", regret);
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
