#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#define chunk 100
#define speed 600

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

long prime = 7;
String primes = "2, 3, 5";

String landingPage = R"===(
  <!DOCTYPE html>
<html>
<head>
<style>
</style>
</head>
<body>

<h1>Parallel Computing</h1><br>
<span>Click <a id="lin">here</a> if you want to join the club.</span>
<script>
document.getElementById("lin").href = window.location.origin+"/comp";
</script>
</body>
</html>
)===";

String compute = R"===(
<h1>COMPUTING</h1><br>
<span>We are up to number <span id="num"></span></span>
<script>
let xhr, resp, i, prime, http, url, params, strpri, i2;
setTimeout(function(){setInterval(function(){
	xhr = new XMLHttpRequest();
    xhr.open("GET", "/pool", true);
    xhr.onreadystatechange = function() {
      if (xhr.readyState == 4 && xhr.status == 200) {
        console.log("good");
        i = Number(xhr.responseText);
        strpri = "";
        i2 = i+)==="+String(chunk)+R"===(;
        for(i=i; i < i2; i+=2){
          prime = 1;
          let sq = Math.sqrt(i)+1;
          for(let k = 3; k < sq; k += 2){
            if(i%k == 0){
              prime = 0;
              break;
            }
          }
          if(prime == 1){
            strpri += ", "+i;
          }
        }
        http = new XMLHttpRequest();
        url = "/result";
        params = "nums="+strpri+"&sta="+xhr.responseText;
        http.open('POST', url, true);
        http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        http.onreadystatechange = function() {
            if(http.readyState == 4 && http.status == 200) {
              document.getElementById("num").innerText = i;
            }
        }
        http.send(params);
      }
    };
    xhr.send(null);
}, )==="+String(speed)+R"===();}, Math.floor(Math.random()*1000)+1000);
</script>
</body>
</html>
)===";

// Handle Root
void defaultPage() {
  webServer.send(200, "text/html", landingPage);
}

void comp() {
  webServer.send(200, "text/html", compute);
}

void pool() {
  webServer.send(200, "text/plain", String(prime));
}

void result() {
  if(webServer.hasArg("nums") && webServer.hasArg("sta")){
    if(webServer.arg("sta").toInt() == prime){
      webServer.send(200, "text/plain", "recieved");
      primes += webServer.arg("nums");
      prime += chunk;
      Serial.println(prime);
    }
  }else{
    webServer.send(200, "text/plain", primes);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Parallel Computing");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound(defaultPage);
  webServer.on("/comp", comp);
  webServer.on("/pool", pool);
  webServer.on("/result", result);
  webServer.begin();
}

void loop() {
  if(Serial.available()){
    prime = (Serial.readString()).toInt();
    if(prime%2 == 0){
      prime++;
    }
    Serial.println("Set prime to be "+String(prime));
    primes += ", ...";
  }
  if (WiFi.softAPgetStationNum() == 0)
  {
    delay(100);
  } else {
    delay(100);
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}