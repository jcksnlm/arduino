int adc;
float voltage;
float current;

void setup() {
  Serial.begin(9600);
}


void loop() {
  adc = analogRead(A7);
  voltage = adc*5/1023.0;
  current = (voltage-2.5)/0.66;
  current = current*10;
  if (current < 0) {
    current = current*-1;
  }
  Serial.println(current);
  if (current > 0.5){
    tone(2, 880, 200); // play tone 69 (A5 = 880 Hz)
    delay(100); // Wait for 200 millisecond(s)
    noTone(2);
  }

  delay(100);
}
