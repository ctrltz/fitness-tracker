int latchPin = 8;   // ST_CP of 74HC595
int clockPin = 12;  // SH_CP of 74HC595
int dataPin = 10;   // DS of 74HC595

int analogIn = A0;  // data pin of pulse sensor
int sensorValue = 0, zeroValue = 525, maxValue = 920;
int interval = 50, previousCheck = 0, previousTick = 0, average_period = 0, average_pulse = 0;
int currentMillis;
int periods[10] = {}, periods_num = 10;
int pulses[5] = {}, pulses_num = 5;
int maximum = LOW;

// binary representations for digit segments
uint8_t binary[10] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246};
uint8_t binary_high[10] = {252, 12, 218, 158, 46, 182, 246, 28, 254, 190};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 

  Serial.begin(9600);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, binary[0]);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

void loop() {
  currentMillis = millis();
  
  if (currentMillis - previousCheck >= interval) {
    sensorValue = analogRead(analogIn);
    //Serial.print("Sensor value ");
    //Serial.println(sensorValue);
    
    if (2 * sensorValue > maxValue + zeroValue) {
      if (!maximum) {
        average_period = 0;
        for (int i = 1; i < periods_num; i++) {
          periods[i] = periods[i-1];
          average_period += periods[i];
          //Serial.print(periods[i]);
          //Serial.print(" ");
        }
        periods[0] = currentMillis - previousTick;
        average_period += periods[0];
        //Serial.print(periods[0]);
        average_period /= periods_num;

        average_pulse = 0;
        for (int i = 1; i < pulses_num; i++) {
          pulses[i] = pulses[i-1];
          average_pulse += pulses[i];
        }
        pulses[0] = float(60000 / average_period);
        average_pulse += pulses[0];
        average_pulse /= pulses_num;
        maximum = HIGH;

        //Serial.print("           Pulse value: ");
        Serial.println(average_pulse);
        
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, binary[int(average_pulse) % 10]);
        shiftOut(dataPin, clockPin, MSBFIRST, binary_high[int(average_pulse / 10) % 10] + (int(average_pulse) > 100));
        digitalWrite(latchPin, HIGH);
        
        previousTick = currentMillis;
      }
    } else {
      maximum = LOW;
    }

    previousCheck = currentMillis;
  }
}
