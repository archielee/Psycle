void setup() {
  Serial.begin(9600);
  vibratorInit();
}

int state = 0, inByte;
const long interval = 500;

void vibratorInit(void) {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  bluetoothCheck();
  
  while(state == 1) // Right turn
    vibrate();
}

void bluetoothCheck(void) {
  if(Serial.available()) {
    inByte = (int)Serial.read();
    inByte -= 48;
    if(inByte == 1) {
      state = inByte;
      Serial.println(state, DEC); // For testing purposes
    }
  }
}

void vibrate(void) {
  unsigned long start = millis();
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  while((millis() - start) < interval) {}
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  state = 0;
}
