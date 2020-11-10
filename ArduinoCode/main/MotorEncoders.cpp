
#include "Arduino.h"
#include "MotorEncoders.h"

volatile long Xposition = 0;
volatile long Yposition = 0;
volatile long Zposition = 0;

volatile int Xdirection = 1;
volatile int Ydirection = 1;
volatile int Zdirection = 1;

int debounceDelayM = 6;

volatile long lastDebounceX = 0;
volatile long lastDebounceY = 0;
volatile long lastDebounceZ = 0;


void x1EncoderISR(void){
  if ((millis() - lastDebounceX) > debounceDelayM) {
    Xposition += Xdirection;
    lastDebounceX = millis();
  }
}

void y1EncoderISR(void){
  if ((millis() - lastDebounceY) > debounceDelayM) {
    Yposition += Ydirection;
    lastDebounceY = millis();
  }
}

void z1EncoderISR(void){
  if ((millis() - lastDebounceZ) > debounceDelayM) {
    Zposition += Zdirection;
    lastDebounceZ = millis();
  }
}

MotorEncoders::MotorEncoders(int encoderX1Pin, int encoderY1Pin, int encoderZ1Pin):
  encoderX1Pin(encoderX1Pin), encoderY1Pin(encoderY1Pin), encoderZ1Pin(encoderZ1Pin){
    
  //La CNC commence toujours à "Home"
  Xposition = 0;
  Yposition = 0;
  Zposition = 0;
}

MotorEncoders::~MotorEncoders(){
  
}

void MotorEncoders::init(){
  pinMode(encoderX1Pin, INPUT_PULLUP);
  attachInterrupt(encoderX1Pin, x1EncoderISR, CHANGE);  
  
  pinMode(encoderY1Pin, INPUT_PULLUP);
  attachInterrupt(encoderY1Pin, y1EncoderISR, CHANGE);
  
  pinMode(encoderZ1Pin, INPUT_PULLUP);
  attachInterrupt(encoderZ1Pin, z1EncoderISR, CHANGE); 
}

long MotorEncoders::getXPosition(){
  return Xposition;
}

long MotorEncoders::setXPosition(long positionInTicks){
  Xposition = positionInTicks;
  return Xposition;
}

long MotorEncoders::getYPosition(){
  return Yposition;
}

long MotorEncoders::setYPosition(long positionInTicks){
  Yposition = positionInTicks;
  return Yposition;
}

long MotorEncoders::getZPosition(){
  return Zposition;
}

long MotorEncoders::setZPosition(long positionInTicks){
  Zposition = positionInTicks;
  return Zposition;
}

void MotorEncoders::setXDirection(byte positive){
  if(positive == 1){
    Xdirection = 1;
  }
  else if(positive ==2){
    Xdirection = -1; 
  }
  else {
    Xdirection = 0;
  }
}

void MotorEncoders::setYDirection(byte positive){
  if(positive == 1){
    Ydirection = 1;
  }
  else if (positive == 2){
    Ydirection = -1; 
  }
  else {
    Ydirection = 0;
  }
}

void MotorEncoders::setZDirection(byte positive){
  if(positive == 1){
    Zdirection = 1;
  }
  else if (positive ==2){
    Zdirection = -1; 
  }
  else {
    Zdirection = 0;
  }
}
