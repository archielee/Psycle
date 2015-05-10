///////////////////////////////////////////////////////////////////////////////////
//********************************LIBRARIES**************************************//
///////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#define PIN1 5
#define PIN2 4
#define LED_COUNT 5
#include <IRremote.h>
#include <SoftwareSerial.h>
#include <toneAC.h>
SoftwareSerial BT(11, 12); 

///////////////////////////////////////////////////////////////////////////////////
//*****************************GLOBAL VARIABLES**********************************//
///////////////////////////////////////////////////////////////////////////////////

int led = 13;//for resetting?
int state = LOW;
int deviceState;
int count = 0;
unsigned long previousMillis = 0;
const long interval = 500;
int RECV_PIN = 8;
char a; // stores incoming character from other device
IRrecv irrecv(RECV_PIN);
decode_results results;
Adafruit_NeoPixel leds1 = Adafruit_NeoPixel(LED_COUNT, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel(LED_COUNT, PIN2, NEO_GRB + NEO_KHZ800);

///////////////////////////////////////////////////////////////////////////////////
//**********************************SETUP****************************************//
///////////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);      //2 left
  pinMode(7, OUTPUT);    //7right
  // set digital pin to control as an output
  pinMode(13, OUTPUT);
  // set the data rate for the SoftwareSerial port
  BT.begin(9600);
  // Send test message to other device
  BT.println("Hello from Arduino");
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(led, OUTPUT);     
  irrecv.enableIRIn(); // Start the receiver
 
}

///////////////////////////////////////////////////////////////////////////////////
//**************************NECESSARY FUNCTIONS**********************************//
///////////////////////////////////////////////////////////////////////////////////


void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds1.setPixelColor(i, 0,0,0);
    leds2.setPixelColor(i, 0,0,0);
  }
  leds1.show();
  leds2.show();
}

void redLEDs(int j)
{
  for (int i=0; i<LED_COUNT; i++)
  {
    if(j==1)
    leds1.setPixelColor(i, 255,0,0);
    else
    leds2.setPixelColor(i, 255,0,0);
      
  }
  leds1.show();
  leds2.show();
}

///////////////////////////////////////////////////////////////////////////////////
//**********************************LOOP*****************************************//
///////////////////////////////////////////////////////////////////////////////////

void loop() {
  if (count == 0)
  {
    if (irrecv.decode(&results)) {
      Serial.println(results.value,HEX);
      if (results.value == 0xFFA25D)
      {
        deviceState = 1;
        
       Serial.println(results.value, HEX);
        count++;
      }
      else if (results.value == 0xFF629D)
      {
      deviceState = 2;
       Serial.println(results.value, HEX);
        count++;
      }
      else if (results.value == 0xFFE21D)
      {
        deviceState = 3;
        Serial.println(results.value, HEX);
        count++;
      }
      else if (results.value == 0xFF22DD)
      {
        deviceState = 4;
        Serial.println(results.value, HEX);
      //irrecv.resume(); // Receive the next value
        count++;
      }
    }
    else
      count=0;
  }
  else
  {
   switch (deviceState)
   {
     case 1://Driving state
         if (BT.available())
      // if text arrived in from BT serial...
      {
        int iniTime = millis();
        int currTime = millis();
        a=(BT.read());
        if (a=='L')
        {
          for (int i = 0; i<10; i++)
          {
            redLEDs(1);
            digitalWrite(2, HIGH);
            delay(400);
            clearLEDs();
            digitalWrite(2, LOW);
            delay(400);   
          }       
        }
        else if (a=='R')
        {
          for (int i = 0; i<10; i++)
          {
          redLEDs(2);
          digitalWrite(7, HIGH);
          delay(400);
          clearLEDs();
          digitalWrite(7, LOW);
          delay(400);
          }
        }
      }
     break;   
     case 2: // parked the vehicle
       irrecv.resume();
       BT.println("locked");
       do{
       a=(BT.read());// you doont have to send out mshs every hour
       BT.println("locked");
       if (irrecv.decode(&results));
       delay(10);
       }while(analogRead(A0) < 620 && analogRead(A1) < 550 && analogRead(A0) > 550 && analogRead(A1) >510);
       BT.println("unlocked");
       count = 0;
       delay(10);
       break;
     case 3:
       digitalWrite(13,HIGH);
       if (irrecv.decode(&results));
       while(!(results.value == 0xFF22DD));
       break;
     case 4:
       toneAC(440); 
       delay(100);
       noToneAC() ;
        delay(100);     
      count = 0;
       break;
     default:
       break;
   }
 }

}
