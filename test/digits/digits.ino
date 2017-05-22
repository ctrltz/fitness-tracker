int latchPin = 8;   // ST_CP of 74HC595
int clockPin = 12;  // SH_CP of 74HC595
int dataPin = 10;   // DS of 74HC595

// binary representations for digit segments
uint8_t binary[10] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
}

void loop() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, binary[i]);
    digitalWrite(latchPin, HIGH);
    delay(1000);
  }
}
