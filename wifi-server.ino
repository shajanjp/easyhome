#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Indominus"; // your connection name
const char* password = "M@keafire"; // your connection password

ESP8266WebServer server(80);

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
  if(state == 1)
    digitalWrite(devices[device], HIGH);
  else
    digitalWrite(devices[device], LOW);
}

void turnEverything(int state){
  if(state == 1){
    for (int i = 0; i < 5; ++i)
      digitalWrite(devices[i], HIGH);
  }
  else{
    for (int i = 0; i < 5; ++i)
      digitalWrite(devices[i], LOW);
  }
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
    server.send(200, "application/json", "{\"devices\": \"success\"}");
    delay(1000);
  });

  // Control Individual devices
  server.on("/devices/1/on", [](){
    digitalWrite(device_1, HIGH);
    server.send(200, "application/json", "{\"state\":true, \"device\": 1}");
    delay(1000);
  });

  server.on("/devices/1/off", [](){
    digitalWrite(device_1, LOW);
    server.send(200, "application/json", "{\"state\":false, \"device\": 1}");
    delay(1000); 
  });

  server.on("/devices/2/on", [](){
    digitalWrite(device_2, HIGH);
    server.send(200, "application/json", "{\"state\":true, \"device\": 2}");
    delay(1000);
  });

  server.on("/devices/2/off", [](){
    digitalWrite(device_2, LOW);
    server.send(200, "application/json", "{\"state\":false, \"device\": 2}");
    delay(1000); 
  });

  server.on("/devices/3/on", [](){
    digitalWrite(device_3, HIGH);
    server.send(200, "application/json", "{\"state\":true, \"device\": 3}");
    delay(1000);
  });

  server.on("/devices/3/off", [](){
    digitalWrite(device_3, LOW);
    server.send(200, "application/json", "{\"state\":false, \"device\": 3}");
    delay(1000); 
  });

  server.on("/devices/4/on", [](){
    digitalWrite(device_4, HIGH);
    server.send(200, "application/json", "{\"state\":true, \"device\": 4}");
    delay(1000);
  });

  server.on("/devices/4/off", [](){
    digitalWrite(device_4, LOW);
    server.send(200, "application/json", "{\"state\":false, \"device\": 4}");
    delay(1000); 
  });

  server.on("/devices/5/on", [](){
    digitalWrite(device_5, HIGH);
    server.send(200, "application/json", "{\"state\":true, \"device\": 5}");
    delay(1000);
  });

  server.on("/devices/5/off", [](){
    digitalWrite(device_5, LOW);
    server.send(200, "application/json", "{\"state\":false, \"device\": 5}");
    delay(1000); 
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