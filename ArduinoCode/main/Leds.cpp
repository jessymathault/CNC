
#include "Arduino.h"
#include "Leds.h"

Leds::Leds(int redPin, int yellowPin, int greenPin):
  redPin(redPin), yellowPin(yellowPin), greenPin(greenPin){
}

Leds::~Leds(){

}

void Leds::init(){
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT); 
  turnAllOff(); 
}

void Leds::setRedLed(bool state){
  if(state == true){
    digitalWrite(redPin, HIGH);
  }
  else{
    digitalWrite(redPin, LOW);
  }
}

void Leds::setYellowLed(bool state){
  if(state == true){
    digitalWrite(yellowPin, HIGH);
  }
  else{
    digitalWrite(yellowPin, LOW);
  } 
}

void Leds::setGreenLed(bool state){
  if(state == true){
    digitalWrite(greenPin, HIGH);
  }
  else{
    digitalWrite(greenPin, LOW);
  } 
}

void Leds::turnAllOff(){
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}

void Leds::turnAllOn(){
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(greenPin, HIGH);
}

