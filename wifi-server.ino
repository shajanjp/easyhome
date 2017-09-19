#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

const char* ssid = "Indominus"; // your connection name
const char* password = "M@keafire"; // your connection password

ESP8266WebServer server(80);

int light_1 = 5;
int light_2 = 4;
int light_3 = 0;
int light_4 = 2;
int light_5 = 14;

//Check if header is present and correct
bool is_authentified(){
  if (server.hasHeader("Cookie")){
    String cookie = server.header("Cookie");
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      return true;
    }
  }
  return false;
}

//login page, also called for disconnect
void handleLogin(){
  String msg;
  if (server.hasHeader("Cookie")){
    String cookie = server.header("Cookie");
  }
  if (server.hasArg("DISCONNECT")){
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.sendHeader("Set-Cookie","ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")){
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "root" ) // enter ur username and password you want
    {
      server.sendHeader("Location","/");
      server.sendHeader("Cache-Control","no-cache");
      server.sendHeader("Set-Cookie","ESPSESSIONID=1");
      server.send(301);
      return;
    }

    msg = "Wrong username/password! try again.";
  }
  String content = "<html><body style='background-color:MediumAquaMarine'><form action='/login' method='POST'><p  align ='center' style='font-size:300%;'><u><b><i>  Log In  </i></b></u></p><br>";
  content += " <p align ='center' style='font-size:160%'><b> UserName:<input type='text' name='USERNAME' placeholder='user name' required></b></p><br>";
  content += "<p align ='center' style='font-size:160%'><b>Password:<input type='password' name='PASSWORD' placeholder='password' required></b></p><br>";
  content += "<p align ='center' style='font-size:160%'><input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "</p><br> </body></html>";
  server.send(200, "text/html", content);
}

//root page can be accessed only if authentification is ok
void handleRoot(){
  String header;
  if (!is_authentified()){
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.send(301);
    return;
  }
  String content =  "<body><h1 align ='center'><b><u><i><strong>HOME AUTOMATION</strong></i></u></b></h1><br><p align ='center'>Switch #1 <a href=\"switch1On\"><button>ON</button></a>&nbsp;<a href=\"switch1Off\"><button>OFF</button></a></p>";
  content += "<br><p  align ='center'>Switch #2 <a href=\"switch2On\"><button>ON</button></a>&nbsp;<a href=\"switch2Off\"><button>OFF</button></a></p>";
  content += "<br><p  align ='center'>Switch #3 <a href=\"switch3On\"><button>ON</button></a>&nbsp;<a href=\"switch3Off\"><button>OFF</button></a></p>";
  content += "<br>";
  content += "<br><br><br><br></body>"; 
  
  if (server.hasHeader("User-Agent")){
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";    
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);
}

//no need authentification
void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void){

   // preparing GPIOs
  pinMode(light_1, OUTPUT);
  pinMode(light_2, OUTPUT);
  pinMode(light_3, OUTPUT);
  pinMode(light_4, OUTPUT);
  pinMode(light_5, OUTPUT);
  
  delay(1000);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works without authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );

  server.on("/test", [](){
    server.send(200, "text/html", "success");
    delay(1000);
  });

  server.on("/devices/1/on", [](){
    digitalWrite(light_1, HIGH);
    server.send(200, "text/html", "on");
    delay(1000);
  });

  server.on("/devices/1/off", [](){
    digitalWrite(light_1, LOW);
    server.send(200, "text/html", "off");
    delay(1000); 
  });

  server.on("/devices/2/on", [](){
    digitalWrite(light_2, HIGH);
    server.send(200, "text/html", "on");
    delay(1000);
  });

  server.on("/devices/2/off", [](){
    digitalWrite(light_2, LOW);
    server.send(200, "text/html", "off");
    delay(1000); 
  });

  server.on("/devices/3/on", [](){
    digitalWrite(light_3, HIGH);
    server.send(200, "text/html", "on");
    delay(1000);
  });

  server.on("/devices/3/off", [](){
    digitalWrite(light_3, LOW);
    server.send(200, "text/html", "off");
    delay(1000); 
  });

  server.on("/devices/4/on", [](){
    digitalWrite(light_4, HIGH);
    server.send(200, "text/html", "on");
    delay(1000);
  });

  server.on("/devices/4/off", [](){
    digitalWrite(light_4, LOW);
    server.send(200, "text/html", "off");
    delay(1000); 
  });

  server.on("/devices/5/on", [](){
    digitalWrite(light_5, HIGH);
    server.send(200, "text/html", "on");
    delay(1000);
  });

  server.on("/devices/5/off", [](){
    digitalWrite(light_5, LOW);
    server.send(200, "text/html", "off");
    delay(1000); 
  });

  server.begin();
}

void loop(void){
  server.handleClient();
}
