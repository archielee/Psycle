#include <Wire.h>
#include <SeeedOLED.h>
#include "Barometer.h"

float temperature;
float pressure;

Barometer barom;

void setup() {
  Wire.begin();
  SeeedOled.init();
  initOledTitle();
  barom.init();
}

void initOledTitle(void){
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,2);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Thermometer"); //Print the String
  SeeedOled.setTextXY(2,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Temp:");       //Print the String
  SeeedOled.setTextXY(3,0);
  SeeedOled.putString("Pres:");
}

void oledMsg() {
  SeeedOled.setTextXY(2,6);
  SeeedOled.putNumber(temperature); //Print the String
  SeeedOled.putString(" deg C");
  SeeedOled.setTextXY(3,6);
  SeeedOled.putNumber(pressure);
  SeeedOled.putString(" Pa");
}

void loop() {
  temperature = barom.bmp085GetTemperature(barom.bmp085ReadUT());
  pressure = barom.bmp085GetPressure(barom.bmp085ReadUP());
  oledMsg();
  delay(500);
}
