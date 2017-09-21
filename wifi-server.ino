#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Indominus"; // your connection name
const char* password = "M@keafire"; // your connection password

ESP8266WebServer server(80);

int device_status[4]; 

int devices[] = {14,12,13,15,3};

//root page can be accessed only if authentification is ok
void handleRoot(){
  server.send(200, "text/html", "{\"message\":\"Welcome to AutoHome\"}");
}

//no need authentification
void handleNotFound(){
  server.send(404, "application/json", "{\"message\":\"Invalid request\"}");
}

void turnDevice(int device, int state){
  if(state == 1){
    digitalWrite(devices[device], HIGH);
    device_status[device] = 1;
  }
  else{
    digitalWrite(devices[device], LOW);
    device_status[device] = 0;
  }
}

void turnEverything(int state){
  if(state == 1){
    for (int i = 0; i < 5; ++i){
      digitalWrite(devices[i], HIGH);
      device_status[i] = 1;
    }
  }
  else{
    for (int i = 0; i < 5; ++i){
      digitalWrite(devices[i], LOW);
      device_status[i] = 0;
    }
  }
}

void handleDevice(int device, int state){
  String response = "{\"state\":";
  response += state;
  response += ", \"device\":";
  response += (device + 1); 
  response += "}";
  turnDevice(device, state);
  server.send(200, "application/json", response);
  delay(1000);
}

void setup(void){
// preparing GPIOs
  for (int i = 0; i < 5; ++i)
    pinMode(devices[i], OUTPUT);

  turnEverything(0);

  delay(1000);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // List all devices
  server.on("/devices", [](){
    String response = "[";
    for (int i = 0; i < 5; ++i){
      response += "{\"state\":";
      response += device_status[i];
      response += ", \"device\":";
      response += (i + 1);
      if(i !=4 )
        response += "},";
      else
        response += "}";
    }
    response += "]";
    server.send(200, "application/json", response);
    delay(1000);
  });

  // Control Individual devices
  server.on("/devices/1/on", [](){
    handleDevice(0, 1);
  });

  server.on("/devices/1/off", [](){
    handleDevice(0, 0);
  });

  server.on("/devices/2/on", [](){
    handleDevice(1, 1);
  });

  server.on("/devices/2/off", [](){
    handleDevice(1, 0);
  });

  server.on("/devices/3/on", [](){
    handleDevice(2, 1);
  });

  server.on("/devices/3/off", [](){
    handleDevice(2, 0);
  });

  server.on("/devices/4/on", [](){
    handleDevice(3, 1);
  });

  server.on("/devices/4/off", [](){
    handleDevice(3, 0);
  });

  server.on("/devices/5/on", [](){
    handleDevice(4, 0);
  });

  server.on("/devices/5/off", [](){
    handleDevice(4, 1);
  });

  server.on("/devices/all/on", [](){
    turnEverything(1);
    server.send(200, "application/json", "{\"state\":false, \"device\": [1,2,3,4,5]}");
    delay(1000); 
  });

  server.on("/devices/all/off", [](){
    turnEverything(0);
    server.send(200, "application/json", "{\"state\":false, \"device\": [1,2,3,4,5]}");
    delay(1000); 
  });

  server.begin();
}

void loop(void){
  server.handleClient();
}