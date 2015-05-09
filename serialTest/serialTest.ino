void setup() {
  Serial.begin(115200);
  vibratorInit();
}

int state = 0, inByte;
const long interval = 500;

void vibratorInit(void) {
  pinMode(12, OUTPUT);
}

void loop() {
  bluetoothCheck();
  
  while(state == 1) // Right turn
    vibrate();
}

void bluetoothCheck(void) {
  if(Serial.available() > 0) {
    inByte = Serial.read();
    inByte -= 48;
    state = inByte;
    Serial.println(state, DEC); // For testing purposes
  }
}

void vibrate(void) {
  unsigned long start = millis();
  digitalWrite(12, HIGH);
  while((millis() - start) < interval) {}
  digitalWrite(12, LOW);
  state = 0;
}

