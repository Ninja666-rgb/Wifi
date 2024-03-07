#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

const char *html = R"===(
<!DOCTYPE html>
<html>
<head>
<title>Don't click</title>
</head>
<body>
<p>DON'T CLICK THE BIG RED BUTTON</p>
<button style="background-color:red; border:none; color:white; font-size:20px;" onclick="window.location = '/regret'">DON'T PRESS</button>
</body>
</html>
)===";

const char *html2 = R"===(
<!DOCTYPE html>
<html>
<head>
<title>Regret</title>
</head>
<body>
<p id = 'reg'>No regert?</p>
<script>
  for(let i = 0; i < 1000; i++){
    document.getElementById('reg').innerHTML = document.getElementById('reg').innerHTML+'<br>No regert?';
  }
  for(let i = 0; i > -1; i*=2){
    location.reload();
  }
</script>
</body>
</html>
)===";

// Handle Root
void defaultPage() {
  webServer.send(200, "text/html", html);
}

void regret() {
  webServer.send(200, "text/html", html2);
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
