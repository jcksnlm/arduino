#include "config.h"
#if defined(ARDUINO_ARCH_ESP32)
  // ESP32Servo Library (https://github.com/madhephaestus/ESP32Servo)
  // installation: library manager -> search -> "ESP32Servo"
  #include <ESP32Servo.h>
#else
  #include <Servo.h>
#endif

// set up the 'digital' feed
AdafruitIO_Feed *digital = io.feed("passar_ibutton");

Servo servoChoose;
Servo servoPress;

unsigned const int servoChoosePin = 16;
unsigned const int servoPressPin = 5;

unsigned const char buttons[4] = {14,2,0,4};

unsigned const int servoChooseIdle = 60;
unsigned const int servoPressPosIdle = 180;
unsigned const int servoPressPosBreakDown = servoPressPosIdle - 145;
unsigned const int servoPressPosPressed = servoPressPosBreakDown - 35;


bool pressingInProgress = false;

int lastPressed = 0;

void setup() {
   
  // start the serial connection
  Serial.begin(9600);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  digital->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  digital->get();

  // setup do hardware
  servoChoose.attach(servoChoosePin, 544, 2400);
  servoPress.attach(servoPressPin, 544, 2400);
  
  for(unsigned int i = 0; i < sizeof(buttons); i++) {
     pinMode(buttons[i], INPUT_PULLUP);
  }

  servoChoose.write(servoChooseIdle);
  servoPress.write(servoPressPosIdle);

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  //configurções botões
  for(unsigned int i = 0; i < sizeof(buttons); i++) {
    if (digitalRead(buttons[i]) == LOW) {
      pressIbutton(i+1);
    }
  }

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.println(data->toInt());
  if (data->toInt()== 0) {
    return;
  }

  pressIbutton(data->toInt());

}

void pressIbutton(int ibuttonSample) {

  unsigned int servoChoosePos;
  unsigned int distanceDelay = abs(lastPressed - ibuttonSample);
  lastPressed = ibuttonSample;
  pressingInProgress = true;
  
  switch (ibuttonSample) {
    case 1:
      servoChoosePos = servoChooseIdle;
      break;
    case 2:
      servoChoosePos = 91;
      break;
    case 3:
      servoChoosePos = 123;
      break;
    case 4:
      servoChoosePos = 180;
      break;
    default:
      servoChoosePos = servoChooseIdle;
      break;
  }

  servoChoose.write(servoChoosePos);
  delay(300 + (distanceDelay * 50));
  
  servoPress.write(servoPressPosBreakDown);
 
  for (int i = servoPressPosBreakDown; i > servoPressPosPressed; i--) {
    servoPress.write(i);
    delay(1);
  }
  
  servoPress.write(servoPressPosPressed);
  delay(2000);

  servoPress.write(servoPressPosIdle);

  pressingInProgress = false;
  
}
