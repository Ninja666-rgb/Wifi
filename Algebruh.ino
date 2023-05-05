#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML;

void defaultPage(){
  randomSeed(millis());
  int ranum = random(-60, 60);
  int ranump = random(-5, 5);
  while(ranump == 0){
    ranump = random(-5, 5);    
  }
  int ranuma = random(-5, 5);
  while(ranuma == 0){
    ranuma = random(-5, 5);    
  }
  int ranumo = random(70);
  responseHTML = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<title>Get algebruh!</title>\n"
                  "</head>\n"
                  "<body>\n"
                  "<p>What is x:<br><span id = equation>equation</span></p>\n"
                  "<form action='/answer'>\n"
                  "<input type='text' id='answer' name='answer'><br><br>\n"
                  "<input type='hidden' id='rightanswer' name='rightanswer' value='"+String(ranum)+"'>\n"
                  "<input type='hidden' id='equation' name='equation' value='"+String(ranump+ranuma)+"x+"+String(ranumo)+"="+String(ranuma)+"x+"+String(ranum*ranump+ranumo)+"'>\n"
                  "<input type='hidden' id='amount' name='amount' value='"+String(ranump)+"'>\n"
                  "<input type='hidden' id='addedx' name='addedx' value='"+String(ranuma)+"'>\n"
                  "<input type='hidden' id='offset' name='offset' value='"+String(ranumo)+"'>\n"
                  "<input type='submit' value='Submit'>\n"
                  "</form>\n"
                  "<script>\n"
                  "var hehenum = "+String(ranum)+";\n"
                  "var hehenump = "+String(ranump)+";\n"
                  "var hehenuma = "+String(ranuma)+";\n"
                  "var heheoff = "+String(ranumo)+";\n"
                  "document.getElementById('equation').innerHTML = (hehenump+hehenuma)+'x+'+heheoff+'='+hehenuma+'x+'+(hehenum*hehenump+heheoff);\n"
                  "</script>\n"
                  "</body>\n"
                  "</html>\n";
  webServer.send(200, "text/html", responseHTML);
}

void answerPage(){
  responseHTML = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<title>Get algebruh!</title>\n"
                  "</head>\n"
                  "<p><span id='tans'></span> <span id='ans'></span></p>\n"
                  "<script>\n"
                  "const queryString = window.location.search;\n"
                  "const urlParams = new URLSearchParams(queryString);\n"
                  "const entered = urlParams.get('answer');\n"
                  "const theans = urlParams.get('rightanswer');\n"
                  "const theeq = urlParams.get('equation');\n"
                  "const xamo = urlParams.get('amount');\n"
                  "const xadd = urlParams.get('addedx');\n"
                  "const offs = urlParams.get('offset');\n"
                  "document.getElementById('tans').innerHTML = entered;\n"
                  "if(entered == theans){\n"
                  "  document.getElementById('ans').innerHTML = 'is correct';\n"
                  "}else{\n"
                  "  document.getElementById('ans').innerHTML = 'is not correct. The correct answer is '+theans+'. How to solve:<br>'+theeq+'<br>Minus '+xadd+'x off of boths sides:<br>'+xamo+'x+'+offs+'='+(theans*xamo+parseInt(offs))+'<br>Minus '+offs+' off of both sides:<br>'+xamo+'x'+'='+(theans*xamo)+'<br>Divide by '+xamo+' on both sides:<br>x='+theans+'<br>So the answer is '+theans+'.<br>';\n"
                  "}\n"
                  "</script>\n"
                  "<form action='/'>\n"
                  "<input type='submit' value='Go back'>\n"
                  "</form>\n"
                  "</body>\n"
                  "</html>\n";
  webServer.send(200, "text/html", responseHTML);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Do not use this wifi");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound(defaultPage);
  webServer.on("/answer", answerPage);
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
