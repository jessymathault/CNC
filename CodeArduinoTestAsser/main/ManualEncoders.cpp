
#include "Arduino.h"
#include "ManualEncoders.h"

void xLeftISR(void){
  //TODO il va falloir détecter la phase avec des variables volatiles qui toggle
  // Pour déterminer la direction

  //Chaque ISR doit modifier la consigne selon un ratio ça va donner une position selon le nombre de tours

  //On va faire ce mode de contrôle à la fin surement
}

void xRightISR(void){

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
  attachInterrupt(pinX1, xLeftISR, CHANGE);  

  pinMode(pinX2, INPUT);
  attachInterrupt(pinX2, xRightISR, CHANGE);

  pinMode(pinY1, INPUT);
  attachInterrupt(pinY1, yLeftISR, CHANGE);

  pinMode(pinY2, INPUT);
  attachInterrupt(pinY2, yRightISR, CHANGE);

  pinMode(pinZ1, INPUT);
  attachInterrupt(pinZ1, zLeftISR, CHANGE);

  pinMode(pinZ2, INPUT);
  attachInterrupt(pinZ2, zRightISR, CHANGE);
}

