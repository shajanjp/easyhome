/*
  08/01/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++
  File: Rainbow.ino
  ------------------------------------------------------------------------
  Description: 
  Demo for NeoPixelBus WS2812 LED Library showcasing a rainbow  pattern
  cycle on a WS2812B LED strip controlled by an ESP8266.  Modified from 
  strandtest.ino in the Adafruit NeoPixel Library.
  ------------------------------------------------------------------------
  Dependencies:
  NeoPixelBus Library -
  https://github.com/Makuna/NeoPixelBus
  ------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 
  https://acrobotic.com/
  ------------------------------------------------------------------------
  License:
  Released under the MIT license. Please check LICENSE.txt for more
  information.  All text above must be included in any redistribution. 
*/

#include <NeoPixelBus.h>

const uint16_t PixelCount = 30;
const uint8_t PixelPin = 14;  // Ignored for Esp8266

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

void setup()
{
    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();
}

RgbColor color;
uint8_t pos;

void loop() {
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