#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "RedAnt"; // wifi SSID
const char* password = "0n3st3p@h3@d"; // wifi password

ESP8266WebServer server(80);

#include <NeoPixelBus.h>
 
const uint16_t PixelCount = 30; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266 and takes RX pin by default

#define colorSaturation 255

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

// Switches d1, d2, d5, d6 || 5, 4, 14, 12
int plugs[] = {5,4,14,12};
int plug_status[4];

// For rainbow
RgbColor color;
uint8_t pos;
RgbColor black(0, 0, 0);

// Home API
void handleRoot(){
  server.send(200, "application/json", "{ \"msg\": \"Welcome to EasyHome\", \"version\": \"1.05 Beta\" }");
}

// 404
void handleNotFound(){
  server.send(404, "application/json", "{ \"msg\": \"Invalid API request\" }");
}

// Strip Success
void handleStripSuccess(){
  server.send(200, "application/json", "{ \"msg\": \"Enjoy the colors\" }");
}

// Set all PINS ON or OFF
void setThese(int start, int end, RgbColor myColor){
  for (int i = start; i <= end; ++i)
  {
    strip.SetPixelColor(i, myColor);
  }
  strip.Show();
}

void rainbow(){
  for(uint16_t j=0; j<256*5; j++) // complete 5 cycles around the color wheel
  { 
    for(uint16_t i=0; i<PixelCount; i++)
    {
      // generate a value between 0~255 according to the position of the pixel
      // along the strip
      pos = ((i*256/PixelCount)+j) & 0xFF;
      // calculate the color for the ith pixel
      color = Wheel( pos );
      // set the color of the ith pixel
      strip.SetPixelColor(i, color);
    }
    strip.Show();
    //strip.Darken(100);
    delay(20);
  }
}

void runThrough(int start, int end, RgbColor myColor, int speed, int direction){
  if (direction == 1) {
    for (int i = start; i <= end; ++i)
    {
      setThese(start, end, RgbColor(0, 0, 0));
      strip.SetPixelColor(i, myColor);
      strip.Show();
      delay(speed);
    }
  }
  else if(direction == 0) {
    for (int j = end; j >= start; j--)
    {
      setThese(start, end, RgbColor(0, 0, 0));
      strip.SetPixelColor(j, myColor);
      strip.Show();
      delay(speed);
    }
  }

}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
RgbColor Wheel(uint8_t WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) 
  {
    return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  } else 
  {
    WheelPos -= 170;
    return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
}

// Swithing plugs
void turnDevice(int plug, int state){
  if(state == 0){
    digitalWrite(plugs[plug], LOW);
    plug_status[plug] = 0;
  }
  else if(state == 1){
    digitalWrite(plugs[plug], HIGH);
    plug_status[plug] = 1;
  }
}

// Handle deivce requests
void handleDevice(int plug, int state){
  turnDevice(plug, state);
  String response = "{\"plug\":";
  response += (plug + 1); 
  response += ", \"state\":";
  response += plug_status[plug];
  response += "}";
  server.send(200, "application/json", response);
  delay(1000);
}

// Main Setup
void setup(void) {

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to...");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("ESP8266")) {
    Serial.println("MDNS responder started...");
  }

  server.on("/", handleRoot);

  server.on("/rainbow", [](){ 
    server.send(200, "application/json", "{ \"msg\": \"Enjoy rainbow colors\" }");
    rainbow();
  });

  server.on("/strip", [](){
    if(server.args() == 3) {
      if(server.arg("r") && server.arg("g") && server.arg("b")) {
        setThese(0, PixelCount - 1, RgbColor(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()));
        server.send(200, "application/json", "{ \"msg\": \"Strip color changed\" }");
      }
      else
        server.send(400, "application/json", "{ \"msg\": \"Invalid Strip API Params\" }");
    }
    else
      server.send(400, "application/json", "{ \"msg\": \"Invalid Strip API Params\" }");
  });

  server.on("/plugs", []() {
    if(server.args() == 2) {
      if(server.arg("plug").toInt() && server.arg("status")) {
        handleDevice(server.arg("plug").toInt() - 1, server.arg("status").toInt());
      }
      else
        server.send(400, "application/json", "{ \"msg\": \"Invalid Plug API Params\" }");
    }
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started...");
  strip.Begin();
  strip.Show();
}

void loop(void){
  server.handleClient();
}