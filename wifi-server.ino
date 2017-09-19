#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

const char* ssid = "Indominus"; // your connection name
const char* password = "M@keafire"; // your connection password

ESP8266WebServer server(80);

int device_1 = 14;
int device_2 = 12;
int device_3 = 13;
int device_4 = 15;
int device_5 = 3;

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
  pinMode(device_1, OUTPUT);
  pinMode(device_2, OUTPUT);
  pinMode(device_3, OUTPUT);
  pinMode(device_4, OUTPUT);
  pinMode(device_5, OUTPUT);
  
  delay(1000);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/login", handleLogin);

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );

  // List all devices
  server.on("/devices", [](){
    server.send(200, "application/json", "{'devices':[device_1,device_2,device_3,device_4,device_5]}");
    delay(1000);
  });

  // Control Individual devices
  server.on("/devices/1/on", [](){
    digitalWrite(device_1, HIGH);
    server.send(200, "application/json", "{'state':true, 'device':1}");
    delay(1000);
  });

  server.on("/devices/1/off", [](){
    digitalWrite(device_1, LOW);
    server.send(200, "application/json", "{'state':false, 'device':1}");
    delay(1000); 
  });

  server.on("/devices/2/on", [](){
    digitalWrite(device_2, HIGH);
    server.send(200, "application/json", "on");
    delay(1000);
  });

  server.on("/devices/2/off", [](){
    digitalWrite(device_2, LOW);
    server.send(200, "application/json", "off");
    delay(1000); 
  });

  server.on("/devices/3/on", [](){
    digitalWrite(device_3, HIGH);
    server.send(200, "application/json", "on");
    delay(1000);
  });

  server.on("/devices/3/off", [](){
    digitalWrite(device_3, LOW);
    server.send(200, "application/json", "off");
    delay(1000); 
  });

  server.on("/devices/4/on", [](){
    digitalWrite(device_4, HIGH);
    server.send(200, "application/json", "on");
    delay(1000);
  });

  server.on("/devices/4/off", [](){
    digitalWrite(device_4, LOW);
    server.send(200, "application/json", "off");
    delay(1000); 
  });

  server.on("/devices/5/on", [](){
    digitalWrite(device_5, HIGH);
    server.send(200, "application/json", "on");
    delay(1000);
  });

  server.on("/devices/5/off", [](){
    digitalWrite(device_5, LOW);
    server.send(200, "application/json", "off");
    delay(1000); 
  });

  server.begin();
}

void loop(void){
  server.handleClient();
}