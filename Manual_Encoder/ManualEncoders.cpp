  //Serial.println("L");
  //TODO il va falloir détecter la phase avec des variables volatiles qui toggle
  // Pour déterminer la direction

  //Chaque ISR doit modifier la consigne selon un ratio ça va donner une position selon le nombre de tours

  //On va faire ce mode de contrôle à la fin surement

#include "Arduino.h"
#include "ManualEncoders.h"

volatile bool senseRotation;
volatile byte action = 0;

long lastDebounceXLeft = 0;  // the last time the output pin was toggled
long lastDebounceXRight = 0;
long lastDebounceYLeft = 0;
long lastDebounceYRight = 0;
long lastDebounceZLeft = 0;
long lastDebounceZRight = 0;
long debounceDelay = 10;    // the debounce time; increase if the output flickers

void xLeftISR(void){

  if ((millis() - lastDebounceXLeft) > debounceDelay) {
    if(senseRotation == 1) 
    {
      senseRotation = 0;
      action = CW;
    }
    else
    {
      senseRotation = 1;
      action = 0;
    }
      lastDebounceXLeft = millis();
  }  

  
}

void xRightISR(void){

  if ((millis() - lastDebounceXRight) > debounceDelay) 
  {
    if(senseRotation == 1) 
      {
        senseRotation = 0;
        action = CCW;
      }
    else
      {
        senseRotation = 1;
        action = 0;
      }
    lastDebounceXRight = millis();
  }    

  
}
  

void yLeftISR(void){
  
}

void yRightISR(void){

}

void zLeftISR(void){

}

void zRightISR(void){

}

ManualEncoders::ManualEncoders(int pinX1, int pinX2, int pinY1, int pinY2, int pinZ1, int pinZ2):
  pinX1(pinX1), pinX2(pinX2), pinY1(pinY1), pinY2(pinY2), pinZ1(pinZ1), pinZ2(pinZ2){  
}

ManualEncoders::~ManualEncoders(){

}

void ManualEncoders::init(){
  pinMode(pinX1, INPUT);
  attachInterrupt(pinX1, xLeftISR, FALLING);  

  pinMode(pinX2, INPUT);
  attachInterrupt(pinX2, xRightISR, FALLING);

  pinMode(pinY1, INPUT);
  attachInterrupt(pinY1, yLeftISR, FALLING);

  pinMode(pinY2, INPUT);
  attachInterrupt(pinY2, yRightISR, FALLING);

  pinMode(pinZ1, INPUT);
  attachInterrupt(pinZ1, zLeftISR, FALLING);

  pinMode(pinZ2, INPUT);
  attachInterrupt(pinZ2, zRightISR, FALLING);
}

int ManualEncoders::debugX1(void)
{
  return action;
}

int ManualEncoders::debugX2(void)
{
  return action;
}

