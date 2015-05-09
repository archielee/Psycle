void setup() {
  Serial.begin(9600);
  vibratorInit();
}

int state = LOW;
unsigned long previousMillis = 0;
const long interval = 500;

void vibratorInit(void) {
  pinMode(10, OUTPUT);      //10, 11 are pins os vibrator 
  pinMode(11, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    if (state == LOW)
      state = HIGH;
    else
      state = LOW;

    digitalWrite(10, state);
    digitalWrite(11, state);
  }
}

