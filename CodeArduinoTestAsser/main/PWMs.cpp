
#include "PWMs.h"
#include "Arduino.h"
#include <assert.h>

#define TIMER_PERIOD_FOR_100KHZ 840 //^Ca nous donne de 0 à 840 de ticks pour le duty

PWMs::PWMs(int pinX1, int pinX2, int pinY1, int pinY2, int pinZ1, int pinZ2):
  pinX1(pinX1), pinX2(pinX2), pinY1(pinY1), pinY2(pinY2), pinZ1(pinZ1), pinZ2(pinZ2){

  //analogWriteResolution(12); //Permet 2^12 = 1024 de résolution vs 2^8 = 256 si on le change pas

  //Les pins 6-9 sur le DUE on 1 controller spécial pour changer la fréquence des PWMs
  pmc_enable_periph_clk(PWM_INTERFACE_ID);
  
  PWMC_ConfigureChannel(PWM, 7, 0, 0, 0);
  PWMC_SetPeriod(PWM, 7, TIMER_PERIOD_FOR_100KHZ);
  PWMC_EnableChannel(PWM, 7);
  
  PWMC_ConfigureChannel(PWM, 6, 0, 0, 0);
  PWMC_SetPeriod(PWM, 6, TIMER_PERIOD_FOR_100KHZ);
  PWMC_EnableChannel(PWM, 6);
  
  PWMC_ConfigureChannel(PWM, 5, 0, 0, 0);
  PWMC_SetPeriod(PWM, 5, TIMER_PERIOD_FOR_100KHZ);
  PWMC_EnableChannel(PWM, 5);
  
  PWMC_ConfigureChannel(PWM, 4, 0, 0, 0);
  PWMC_SetPeriod(PWM, 4, TIMER_PERIOD_FOR_100KHZ);
  PWMC_EnableChannel(PWM, 4);

  //TODO Changer le timer pour mettre le signal en Z à 100KHz aussi
}

PWMs::~PWMs(){
  //On shut down les moteurs préventivement
  analogWrite(pinX1, 0);
  analogWrite(pinX2, 0);
  analogWrite(pinY1, 0);
  analogWrite(pinY2, 0);
  analogWrite(pinZ1, 0);
  analogWrite(pinZ2, 0);  
}

void PWMs::init(){
  pinMode(pinX1,  OUTPUT);
  pinMode(pinX2,  OUTPUT);
  pinMode(pinY1,  OUTPUT);
  pinMode(pinY2,  OUTPUT);
  pinMode(pinZ1,  OUTPUT);
  pinMode(pinZ2,  OUTPUT);

  //Met tout à 0 au cas où, set des registres nécessaires implicitement du même coup
  analogWrite(pinX1, 0);
  analogWrite(pinX2, 0);
  analogWrite(pinY1, 0);
  analogWrite(pinY2, 0);
  analogWrite(pinZ1, 0);
  analogWrite(pinZ2, 0);  
}

void PWMs::setDuty(E_CHANNEL channel, int ticks){
  //On limite les inputs à 0-100% du duty
  if(ticks < 0){
    ticks = 0;  
  }
  else if(ticks > TIMER_PERIOD_FOR_100KHZ){
    ticks = TIMER_PERIOD_FOR_100KHZ; 
  }
  
  switch(channel){
    case E_X1:
      PWMC_SetDutyCycle(PWM, 6, ticks);
      break;
    case E_X2:
      PWMC_SetDutyCycle(PWM, 7, ticks);
      break;
    case E_Y1:
      PWMC_SetDutyCycle(PWM, 5, ticks);
      break;
    case E_Y2:
      PWMC_SetDutyCycle(PWM, 4, ticks);
      break;
    case E_Z1:
      analogWrite(pinZ1, ticks);
      break;
    case E_Z2:
      analogWrite(pinZ2, ticks);
      break;
    default:
      assert(0);
      break;
  }
}

