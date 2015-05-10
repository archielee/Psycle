#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); 
void setup() {
  Serial.begin(9600);
  vibratorInit();
  // set digital pin to control as an output
  pinMode(13, OUTPUT);
  // set the data rate for the SoftwareSerial port
  BT.begin(9600);
  // Send test message to other device
  BT.println("Hello from Arduino");
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
}

int state = LOW;
unsigned long previousMillis = 0;
const long interval = 500;

void vibratorInit(void) {
  pinMode(2, OUTPUT);      //10 left
  pinMode(3, OUTPUT);    //11 right
}

char a; // stores incoming character from other device

void loop() {
  if (BT.available())
    // if text arrived in from BT serial...
    {
      a=(BT.read());
      if (a=='L')
      {
        digitalWrite(13, HIGH);
        digitalWrite(2, HIGH);
        BT.println("LED on");
        delay(500);
        digitalWrite(13, LOW);
        digitalWrite(2, LOW);
      }
      else if (a=='R')
      {
        digitalWrite(13, LOW);
        digitalWrite(3, HIGH);
        BT.println("LED off");
        delay(500);
        digitalWrite(3, LOW);
      }
      else if (a=='?')
      {
        BT.println("Send '1' to turn LED on");
        BT.println("Send '2' to turn LED on");
      }   
      // you can add more "if" statements with other characters to add more commands
    }
    
  }


