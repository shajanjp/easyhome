// NeoPixelBus by Makuna - Version: 2.6.0
#include <NeoPixelBus.h>

// IRremoteESP8266 - Version: 2.8.2
#include <IRrecv.h>

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

uint16_t RECV_PIN = D4; // GPIO 2
IRrecv irrecv(RECV_PIN);
decode_results results;

const char *ssid = "Redant";          // wifi SSID
const char *password = "hellogwc404"; // wifi password

ESP8266WebServer server(80);


const uint16_t PixelCount = 24; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;     // make sure to set this to the correct pin, ignored for Esp8266 and takes RX pin by default

#define colorSaturation 255

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

// Switches d1, d2, d5, d6 || 5, 4, 14, 12
int devices[] = {5, 4, 14, 12};
int device_status[4];

// For rainbow
RgbColor color;
uint8_t pos;
RgbColor black(0, 0, 0);

MDNSResponder mdns;

// Home API
void handleRoot()
{
  server.send(200, "application/json", "{ \"messsge\": \"Welcome to EasyHome\", \"version\": \"1.0 Beta\", \"docs\": \"http://github.com/shajanjp/easyhome\" }");
}

// 404
void handleNotFound()
{
  server.send(404, "application/json", "{ \"message\": \"Not Found\" }");
}

// Strip Success
void handleStripSuccess()
{
  server.send(200, "application/json", "{ \"message\": \"Enjoy the colors\" }");
}

// Set all PINS ON or OFF
void setThese(int start, int end, RgbColor myColor)
{
  for (int i = start; i <= end; ++i)
  {
    strip.SetPixelColor(i, myColor);
  }
  strip.Show();
}

void rainbow()
{
  for (uint16_t j = 0;; j++)
  {
    for (uint16_t i = 0; i < PixelCount; i++)
    {
      pos = ((i * 256 / PixelCount) + j) & 0xFF;
      color = Wheel(pos);
      strip.SetPixelColor(i, color);
    }
    strip.Show();
    delay(20);
  }
}

void runThrough(int start, int end, RgbColor myColor, int speed, int count, bool doReverse)
{
  for (int c = 0; c < count; ++c)
  {
    for (int i = start; i <= end; ++i)
    {
      setThese(start, end, RgbColor(0, 0, 0));
      strip.SetPixelColor(i, myColor);
      strip.Show();
      delay(speed);
    }
    if(doReverse){
      for (int j = end; j >= start; j--)
      {
        setThese(start, end, RgbColor(0, 0, 0));
        strip.SetPixelColor(j, myColor);
        strip.Show();
        delay(speed);
      }
    }
    setThese(start, end, black);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
RgbColor Wheel(uint8_t WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

// Swithing devices
void turnDevice(int device, int state)
{
  if (state == 0)
  {
    digitalWrite(devices[device], LOW);
    device_status[device] = 0;
  }
  else if (state == 1)
  {
    digitalWrite(devices[device], HIGH);
    device_status[device] = 1;
  }
}

// Handle deivce requests
void handleDevice(int device, int state)
{
  turnDevice(device, state);
  String response = "{\"device\":";
  response += (device + 1);
  response += ", \"status\":";
  response += device_status[device];
  response += "}";
  server.send(200, "application/json", response);
  delay(1000);
}

using namespace websockets;
WebsocketsClient client;

// Main Setup
void setup(void)
{
  irrecv.enableIRIn();

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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to...");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("ESP8266"), WiFi.localIP())
  {
    Serial.println("MDNS responder started...");
  }

  MDNS.addService("http", "tcp", 80);
  server.on("/", handleRoot);

  server.on("/rainbow", []() {
    server.send(200, "application/json", "{ \"message\": \"Enjoy rainbow colors\" }");
    return rainbow();
  });

  server.on("/strip", []() {
    if (server.arg("r") != "" && server.arg("g") != "" && server.arg("b") != "")
    {
      handleStripSuccess();
      if (server.arg("effect") == "roll")
        return runThrough(0, PixelCount - 1, RgbColor(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()), 10, 3, false);
      if (server.arg("effect") == "snake")
        return runThrough(0, PixelCount - 1, RgbColor(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()), 100, 1, false);
      if (server.arg("effect") == "flat")
        return setThese(0, PixelCount - 1, RgbColor(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()));
      else
        return setThese(0, PixelCount - 1, RgbColor(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt()));
    }
    else
      handleNotFound();
  });

  server.on("/devices", []() {
    if (server.args() == 2)
    {
      if (server.arg("device").toInt() && server.arg("status"))
      {
        handleDevice(server.arg("device").toInt() - 1, server.arg("status").toInt());
      }
      else
        handleNotFound();
    }
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started...");
  strip.Begin();
  strip.Show();

  runThrough(0, PixelCount - 1, RgbColor(200, 200, 200), 10, 1, true);

  client.connect("ws://easyhome-server.glitch.me/ws");
  client.send("Hello from nodemcu");
  client.onMessage([](WebsocketsMessage msg) {
    Serial.println("Got Message: " + msg.data());
    if (msg.data() == "VISIT")
    {
      runThrough(0, PixelCount - 1, RgbColor(0, 250, 0), 50, 3, false);
    }
    else if (msg.data() == "CLAPS")
    {
      runThrough(0, PixelCount - 1, RgbColor(250, 250, 0), 20, 5, false);
    }
    else
    {
      runThrough(0, PixelCount - 1, RgbColor(50, 50, 50), 10, 1, false); 
    }
  });
}

void handleIrRequest(int code)
{
  switch (code)
  {
  case 16748655: // red
    setThese(0, PixelCount - 1, RgbColor(255, 0, 0));
    break;

  case 16716015: // green
    setThese(0, PixelCount - 1, RgbColor(0, 255, 0));
    break;

  case 16732335: // blue
    setThese(0, PixelCount - 1, RgbColor(0, 0, 245));
    break;

  case 255: // white
    setThese(0, PixelCount - 1, RgbColor(200, 200, 200));
    break;

  case 16756815: // orange
    setThese(0, PixelCount - 1, RgbColor(255, 165, 0));
    break;

  case 16724175: // light green
    setThese(0, PixelCount - 1, RgbColor(144, 238, 144));
    break;

  case 16740495: // dark blue
    setThese(0, PixelCount - 1, RgbColor(0, 0, 139));
    break;

  case 16754775: // dark yellow
    setThese(0, PixelCount - 1, RgbColor(246, 109, 48));
    break;

  case 16722135: // cyan
    setThese(0, PixelCount - 1, RgbColor(0, 255, 255));
    break;

  case 16738455: // lyon blue
    setThese(0, PixelCount - 1, RgbColor(0, 191, 255));
    break;

  case 16750695: // yellow
    setThese(0, PixelCount - 1, RgbColor(247, 155, 40));
    break;

  case 16718055: // light blue
    setThese(0, PixelCount - 1, RgbColor(173, 216, 230));
    break;

  case 16734375: // purple
    setThese(0, PixelCount - 1, RgbColor(128, 0, 128));
    break;

  case 16746615: // light yellow
    setThese(0, PixelCount - 1, RgbColor(255, 255, 0));
    break;

  case 16713975: // skyblue
    setThese(0, PixelCount - 1, RgbColor(135, 206, 235));
    break;

  case 16730295: // red dish
    setThese(0, PixelCount - 1, RgbColor(255, 20, 147));
    break;

  case 16762935: // rainbow
    rainbow();
    break;

  case 16736415: // black
    setThese(0, PixelCount - 1, RgbColor(0, 0, 0));
    break;

  case 16769055: // white animation
    runThrough(0, PixelCount - 1, RgbColor(200, 200, 200), 10, 1, true);
    break;

  default:
    setThese(0, PixelCount - 1, RgbColor(255, 255, 255));
    break;
  }
}

void loop(void)
{
  server.handleClient();

  if (irrecv.decode(&results))
  {
    unsigned int ircode = results.value;
    handleIrRequest(ircode);
    irrecv.resume();
  }

  client.poll();
  delay(100);
}