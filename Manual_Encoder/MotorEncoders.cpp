
#include "Arduino.h"
#include "MotorEncoders.h"

volatile long Xposition = 0;
volatile long Yposition = 0;
volatile long Zposition = 0;

volatile int Xdirection = 1;
volatile int Ydirection = 1;
volatile int Zdirection = 1;

void x1EncoderISR(void){
  Xposition += Xdirection;
}

void y1EncoderISR(void){
  Yposition += Ydirection;
}

void z1EncoderISR(void){
  Zposition += Zdirection;
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

void MotorEncoders::setXDirection(boolean positive){
  if(positive){
    Xdirection = 1;
  }
  else{
    Xdirection = -1; 
  }
}

void MotorEncoders::setYDirection(boolean positive){
  if(positive){
    Ydirection = 1;
  }
  else{
    Ydirection = -1; 
  }
}

void MotorEncoders::setZDirection(boolean positive){
  if(positive){
    Zdirection = 1;
  }
  else{
    Zdirection = -1; 
  }
}
