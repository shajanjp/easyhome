#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "GWC"; // your connection name
const char* password = "shaj1234"; // your connection password

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

void blinkDevice(int device){
    device_status[device] = 2;
    digitalWrite(devices[device], HIGH);
    delay(1000);
    digitalWrite(devices[device], LOW); 
    delay(1000);
    digitalWrite(devices[device], HIGH);
    delay(1000);
    digitalWrite(devices[device], LOW); 
    delay(1000);
    digitalWrite(devices[device], HIGH);
    delay(1000);
    digitalWrite(devices[device], LOW); 
    delay(1000);
}

void turnDevice(int device, int state){
  if(state == 0){
    digitalWrite(devices[device], LOW);
    device_status[device] = 0;
  }
  else if(state == 1){
    digitalWrite(devices[device], HIGH);
    device_status[device] = 1;
  }
  else(state == 2){
    blinkDevice(device);
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
  String response = "{\"device\":";
  response += (device + 1); 
  response += ", \"state\":";
  response += state;
  response += "}";
  turnDevice(device, state);
  server.send(200, "application/json", response);
  delay(1000);
}

void handleDeviceStatus(){
  String response = "[";
  for (int i = 0; i < 5; ++i){
    response += "{\"device\":";
    response += (i + 1);
    response += ", \"status\":";
    response += device_status[i];
    if(i !=4 )
      response += "},";
    else
      response += "}";
  }
  response += "]";
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
  
  // Control Individual devices
  server.on("/devices", [](){
    if(server.args() == 2){
      if(server.arg("device") == "all"){
        turnEverything(server.arg("status").toInt());
        handleDeviceStatus();
      }
      else
        handleDevice((server.arg("device").toInt() - 1), server.arg("status").toInt());
    }
    else
      handleDeviceStatus();
  });
  server.begin();
}

void loop(void){
  server.handleClient();
}