#include <Wire.h>
#include <SeeedOLED.h>
#include "Barometer.h"
#include <Time.h>

float temperature;
float pressure;
time_t t;

Barometer barom;

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
  SeeedOled.setTextXY(5,0);
  SeeedOled.putString("Time:");
  SeeedOled.setTextXY(6,0);
  SeeedOled.putString("Date:");
}

void setup() {
  Wire.begin();
  SeeedOled.init();
  initOledTitle();
  t = now();
  barom.init();
}

void oledMsg() {
  SeeedOled.setTextXY(2,6);
  SeeedOled.putNumber(temperature); //Print the String
  SeeedOled.putString(" deg C");
  SeeedOled.setTextXY(3,6);
  SeeedOled.putNumber(pressure);
  SeeedOled.putString(" Pa");
  SeeedOled.setTextXY(5,6);
  SeeedOled.putNumber(hour(t));
  SeeedOled.setTextXY(5,9);
  SeeedOled.putNumber(minute(t));
  SeeedOled.setTextXY(5,12);
  SeeedOled.putNumber(second(t));
  SeeedOled.setTextXY(6,6);
  SeeedOled.putNumber(month(t));
  SeeedOled.setTextXY(6,9);
  SeeedOled.putNumber(day(t));
  SeeedOled.setTextXY(6,12);
  SeeedOled.putNumber(year(t));
}

void loop() {
  t = now();
  temperature = barom.bmp085GetTemperature(barom.bmp085ReadUT());
  pressure = barom.bmp085GetPressure(barom.bmp085ReadUP());
  oledMsg();
  delay(1000);
}
