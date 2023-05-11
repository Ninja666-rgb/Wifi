#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String messages, messages2, names, kick;

void chatstart() {
  webServer.send(302, "text/html", String(F("<!DOCTYPE>  <html> <head> <title>Chatting.com</title> </head> <body> <h1>Welcome to chatting.com</h1> <label>Username:</label> <input type='text' id='name' value='Anonymous_Capybara'><br> <button onclick=thing()>Go chatting!</button><br><span id = 'er'></span> <script>   function thing(){   let name = document.getElementById('name').value;   if(name.includes(' ')){               document.getElementById('er').innerText = 'Spaces are not allowed';     return 0;   }   if(name.length < 4){     document.getElementById('er').innerText = 'Too short. Please use a username longer than 3 characters.';     return 0;   }   if(name.toUpperCase().includes('ADMIN')){     document.getElementById('er').innerText = 'You are not the admin so please do not act like you are.';     return 0;   }   let names = ["))+names+String(F("];   names = names.map(x => x.toUpperCase());   if(names.includes(name.toUpperCase())){     document.getElementById('er').innerText = 'This name has already been taken';     return 0;   }   window.location = '/chat?name='+name;   } </script>  </body> </html>")));
  webServer.client().stop();
}

void chat(){
  if(names.indexOf("'"+webServer.arg("name")+"', ") == -1){
    names += "'"+webServer.arg("name")+"', ";
    messages += webServer.arg("name")+" joined the chat<br>";
    messages2 = messages;
    messages2.replace("<br>","\n");
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println(messages2);
  }
  if(kick.indexOf(" "+webServer.arg("name")+" ") > -1){
    webServer.send(302, "text/html", String(F("<!DOCTYPE html> <html> <head> <title>Chatting.com</title> </head> <body> <h1>You have been kicked!</h1> <script>setTimeout(window.location.reload(), 3000);</script></body> </html>")));
  }else{
    webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    webServer.sendHeader("Pragma", "no-cache");
    webServer.sendHeader("Expires", "-1");
    webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    webServer.send(302, "text/html", "");
    webServer.sendContent(String(F("<!DOCTYPE html> <html> <head> <title>Chatting.com</title> </head> <body> <h1>Chat</h1> <p>Username: <span id = 'name'></span> <br><br></p><p id = 'mess'>"))+messages+String(F("</p> <input type='text' id='message'><br> <button onclick='thing()'>Send</button> <script>setInterval(function(){const Http = new XMLHttpRequest(); const url=window.location.origin+'/api'; Http.open(\"GET\", url); Http.send(); Http.onreadystatechange = (e) => {   document.getElementById('mess').innerHTML = Http.responseText;  let text = document.getElementById('mess').innerHTML; if(text.includes(\"ADMIN: /kick "))+String(webServer.arg("name"))+String(F("\")){  window.location.reload();} document.getElementById('mess').innerHTML = text.replace(/(ADMIN: \\/rickroll)/g, \"ADMIN: /rickroll<br> <img src='data:image/jpeg;base64,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")));
    webServer.sendContent(F("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' style='height: 600px; width: 600px;' />\");}}, 3000); let params = new URL(document.location).searchParams; document.getElementById('name').innerText = params.get('name'); function thing(){ window.location = '/send?name='+params.get('name')+'&message='+document.getElementById('message').value; } </script> </body> </html>"));
  }
  webServer.client().stop();
}

void sender(){
  if(kick.indexOf(" "+webServer.arg("name")+" ") > -1){
    webServer.send(302, "text/html", String(F("<!DOCTYPE html> <html> <head> <title>Chatting.com</title> </head> <body> <h1>You have been kicked</h1> <script>setInterval(window.location.reload(), 3000);</script></body> </html>")));
  }else{
    messages += webServer.arg("name")+": "+webServer.arg("message")+"<br>";
    webServer.send(302, "text/html", String(F("<!DOCTYPE html> <html> <head> <title>Sending...</title> </head> <body> <h1>Sending...</h1> <script> let params = new URL(document.location).searchParams; setTimeout(function(){window.location = '/chat?name='+params.get('name');}, 1000); </script> </body> </html>")));
    webServer.client().stop();
    messages2 = messages;
    messages2.replace("<br>","\n");
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println(messages2);
  }
}

void messageupdate(){
  webServer.send(200, "text/plain", messages);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("chat.com", "chatting");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound(chatstart);
  webServer.on("/chat", chat);
  webServer.on("/send", sender);
  webServer.on("/api", messageupdate);
  webServer.begin();
  Serial.begin(115200);
}

void loop() {
  if(Serial.available()){
    String said = Serial.readStringUntil('\n');
    if(said.indexOf("/kick ") == 0){
      kick += " "+said.substring(6,said.length())+" ";
    }
    if(said.indexOf("/unkick ") == 0){
      kick.replace(" "+said.substring(8,said.length())+" ", "");
    }
    messages += "ADMIN: "+said+"<br>";
    if(said == "/clear"){
      messages = "Chat history cleared<br>";
    }
    messages2 = messages;
    messages2.replace("<br>","\n");
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println(messages2);
  }
  if (WiFi.softAPgetStationNum() == 0)
  {
    delay(50);
  } else {
    delay(50);
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}
