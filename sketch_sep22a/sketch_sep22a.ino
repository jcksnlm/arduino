#include <Servo.h>
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

  servoChoose.attach(servoChoosePin, 544, 2400);
  servoPress.attach(servoPressPin, 544, 2400);
  
  for(unsigned int i = 0; i < sizeof(buttons); i++) {
     pinMode(buttons[i], INPUT_PULLUP);
  }

   servoChoose.write(servoChooseIdle);
  servoPress.write(servoPressPosIdle);
  
}

void loop() {
  if (!pressingInProgress) {
    for(unsigned int i = 0; i < sizeof(buttons); i++) {
      if (digitalRead(buttons[i]) == LOW) {
        pressIbutton(i+1);
      }
    }
  }
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
      servoChoosePos = 90;
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
  delay(300 + (distanceDelay * 150));
  
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

//
//void  moveChooseServo(int posFinal) {
//  int posAtual = servoChoose.read();
//  int incremento = (posFinal > posAtual) ? 1 : -1; // Determina se estamos aumentando ou diminuindo a posição
//  
//
//  while (posAtual != posFinal) {
//    posAtual += incremento;
//    servoChoose.write(posAtual);
////    delay(5);
//  }
//}
