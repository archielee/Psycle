/* SparkFun WS2812 Breakout Board Example
  SparkFun Electronics
  date: July 25, 2013
  license: GNU GENERAL PUBLIC LICENSE
  
  Requires the Adafruit NeoPixel library. It's awesome, go get it.
  https://github.com/adafruit/Adafruit_NeoPixel
  
  This simple example code runs three sets of animations on a group of WS2812
  breakout boards. The more boards you link up, the better these animations
  will look. 
  
  For help linking WS2812 breakouts, checkout our hookup guide:
  https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide
  
  Before uploading the code, make sure you adjust the two defines at the
  top of this sketch: PIN and LED_COUNT. Pin should be the Arduino pin
  you've got connected to the first pixel's DIN pin. By default it's
  set to Arduino pin 4. LED_COUNT should be the number of breakout boards
  you have linked up.
*/
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN1 3
#define PIN2 2
#define LED_COUNT 5

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds1 = Adafruit_NeoPixel(LED_COUNT, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel(LED_COUNT, PIN2, NEO_GRB + NEO_KHZ800);

void setup()
{
  leds1.begin();  // Call this to start up the LED strip.
  leds2.begin();
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds1.show();  // ...but the LEDs don't actually update until you call this.
  leds2.show();
}

void loop()
{
  redLEDs();
}
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds1.setPixelColor(i, 0);
    leds2.setPixelColor(i, 0);
  }
}

void redLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds1.setPixelColor(i, 255,0,0);
    leds2.setPixelColor(i, 255,0,0);
  }
  leds1.show();
  leds2.show();
}


