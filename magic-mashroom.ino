// NeoPixelTest
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.
//
// Included but commented out are examples of configuring a NeoPixelBus for
// different color order including an extra white channel, different data speeds, and
// for Esp8266 different methods to send the data.
// NOTE: You will need to make sure to pick the one for your platform 
//
//
// There is serial output of the current state so you can confirm and follow along
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 30; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 255

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod> strip(PixelCount, PixelPin);

// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
//
// NeoEsp8266Uart800KbpsMethod uses GPI02 instead

// You can also use one of these for Esp8266, 
// each having their own restrictions
//
// These two are the same as above as the DMA method is the default
// NOTE: These will ignore the PIN and use GPI03 pin
// NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);
// NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma400KbpsMethod> strip(PixelCount, PixelPin);

// Uart method is good for the Esp-01 or other pin restricted modules
// NOTE: These will ignore the PIN and use GPI02 pin
// NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
// NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart400KbpsMethod> strip(PixelCount, PixelPin);

// The bitbang method is really only good if you are not using WiFi features of the ESP
// It works with all but pin 16
// NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(PixelCount, PixelPin);
// NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang400KbpsMethod> strip(PixelCount, PixelPin);

// four element pixels, RGBW
// NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor whatsapp(50, 208, 50);
RgbColor white(colorSaturation);
RgbColor black(0);

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();

    int r=0;
    int g=0;
    int b=0;
}

void macBlur(){

}

RgbColor nextColor() {

}

void rainbow(int delay_time){
    int r=0;
    int g=0;
    int b=0;

for (r = 0; r <= 255; r+=10){
    setThese(0, 29, RgbColor(r,g,b)); //300
    delay(delay_time);
}
r=255;

for (g = 0; g <= 255; g+=10){
    setThese(0, 29, RgbColor(r,g,b)); //330
    delay(delay_time);
}
g=255;

for (r = 255; r >= 0; r-=10){
    setThese(0, 29, RgbColor(r,g,b)); //030
    delay(delay_time);
}
r=0;

for (b = 0; b <= 255; b+=10){
    setThese(0, 29, RgbColor(r,g,b)); //033
    delay(delay_time);
}
b=255;

for (g = 255; g >= 0; g-=10){
    setThese(0, 29, RgbColor(r,g,b)); //003
    delay(delay_time);
}
g=0;

for (r = 0; r <= 255; r+=10){
    setThese(0, 29, RgbColor(r,g,b)); //303
    delay(delay_time);
}
r=255;

for (b = 255; b >= 0; b-=10){
    setThese(0, 29, RgbColor(r,g,b)); //300
    delay(delay_time);
}
b=0;

for (r = 255; r >= 0; r-=10){
    setThese(0, 29, RgbColor(r,g,b)); //000
    delay(delay_time);
}
r=0;

}

void setThese(int start, int end, RgbColor myColor){
    for (int i = start; i <= end; ++i)
    {
        strip.SetPixelColor(i, myColor);
    }
    strip.Show();
}

void makeThisSet(int leds[], RgbColor Color){
    for (int i = 0; i < 15; ++i)
    {     
        strip.SetPixelColor(leds[i], Color);
        strip.Show();
    }
}

void runThrough(int start, int end, RgbColor myColor, int speed, int direction){
    if (direction == 1) {
        for (int i = start; i <= end; ++i)
        {
            setThese(start, end, black);
            strip.SetPixelColor(i, myColor);
            strip.Show();
            delay(speed);
        }
    }
    else if(direction == 0) {
        for (int j = end; j >= start; j--)
        {
            setThese(start, end, black);
            strip.SetPixelColor(j, myColor);
            strip.Show();
            delay(speed);
        }
    }

}

void loop()
{
    rainbow(50);
}