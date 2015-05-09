void setup() {
  Serial.begin(115200);
  vibratorInit();
}

int state = LOW, inByte = -1;
unsigned long prev = 0;
const long interval = 500;

void vibratorInit(void) {
  pinMode(10, OUTPUT);      // 10, 11 are pins for vibrator 
  pinMode(11, OUTPUT);
}

void loop() {
  bluetoothCheck();
  
  if(inByte == 1) // Right turn
    vibrate();
}

void bluetoothCheck(void) {
  if(Serial.available() > 0) {
    inByte = Serial.read();
    
    Serial.println(inByte, DEC); // For testing purposes
  }
}

void vibrate(void) {
  unsigned long runtime = 0;
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  while(runtime < interval) {
    runtime = millis();
  }
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}
