
const int buttonPin = 10;
const int ledPin =  5;

int buttonState = 0;
int ledActivate = 0;
bool releasedButton = true;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW && releasedButton == true) {
    releasedButton = false;
    toggleLed();
  }
  else if (buttonState == HIGH && releasedButton == false) {
    releasedButton = true;
  }
  
  digitalWrite(ledPin, ledActivate);
  delay(5);
}

void toggleLed() {
  if (ledActivate == HIGH) {
    ledActivate = LOW;
  }
  else {
    ledActivate = HIGH;
  }
}
