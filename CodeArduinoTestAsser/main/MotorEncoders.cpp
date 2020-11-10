
#include "Arduino.h"
#include "MotorEncoders.h"

volatile long Xposition = 0;
volatile long Yposition = 0;
volatile long Zposition = 0;

void x1EncoderISR(void){
  Xposition++;
}

void y1EncoderISR(void){
  Yposition++;
}

void z1EncoderISR(void){
  Zposition++;
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

void MotorEncoders::setXPosition(long positionInTicks){
  Xposition = positionInTicks;
}

long MotorEncoders::getYPosition(){
  return Yposition;
}

void MotorEncoders::setYPosition(long positionInTicks){
  Yposition = positionInTicks;
}

long MotorEncoders::getZPosition(){
  return Zposition;
}

void MotorEncoders::setZPosition(long positionInTicks){
  Zposition = positionInTicks;
}



