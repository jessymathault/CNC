
#include "Arduino.h"
#include <assert.h>

#include "CNC.h"
#include "Leds.h"
#include "LimitSwitches.h"
#include "ManualEncoders.h"
#include "MotorEncoders.h"
#include "PID.h"
#include "PWMs.h"

//Périphériques 
Leds* leds = new Leds(51, 50, 52);
LimitSwitches* switches = new LimitSwitches(29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40);
PWMs* pwms = new PWMs(7, 6, 9, 8, 4, 5);
MotorEncoders* encoders = new MotorEncoders(43, 42, 41);
ManualEncoders* manualEncoders = new ManualEncoders(49, 48, 47, 46, 45, 44);

//La classe CNC agit comme controller
CNC* cnc = new CNC(pwms, encoders, switches, leds);

long timeNow = 0;
long lastTime = 0;

void setup()
{ 
  Serial.begin(115200);
  
  leds->init();
  switches->init();
  encoders->init();
  pwms->init();
  cnc->init();

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop()
{
  if(switches->anyIsPressed()){
    cnc->setCurrentState(E_ERROR); 
  }
  
  if((E_CNC_STATES)cnc->getCurrentState() == E_ERROR){
    leds->setRedLed(true);
    cnc->stop();
    return;
  }
  else if((E_CNC_STATES)cnc->getCurrentState() == E_BUSY){
    leds->setYellowLed(true);
    leds->setGreenLed(false);
    cnc->lockedLoop();
  }
  else if((E_CNC_STATES)cnc->getCurrentState() == E_IDLE){
    leds->setYellowLed(false);
    leds->setGreenLed(true);
    //On attend des commandes sur le port série
    timeNow = millis();
    if (timeNow >= lastTime+20)
    {
      lastTime = timeNow;
      Serial.print(timeNow);
      Serial.print(" - ");
      Serial.print(encoders->getXPosition());
      Serial.print(" - ");
      Serial.print(encoders->getYPosition());
      Serial.print(" - ");
      Serial.println(encoders->getZPosition());
    }
    

    
  }
  else{
    assert(0);
  }
}

//Appelé à chaque loop() si le buffer du UART a des données
void serialEvent() {

  //Pour faire des tests ou calibrer l'asservissement
  int command = Serial.read();
  if(command == 'x'){
    pwms->setDuty(E_X1, 200);
    pwms->setDuty(E_X2, 0); 
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Start_x1");
  }
  if(command == 'c'){
    pwms->setDuty(E_X1, 0);
    pwms->setDuty(E_X2, 200); 
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Start_x2");
  }

  if(command == 'y'){
    pwms->setDuty(E_Y1, 200);
    pwms->setDuty(E_Y2, 0); 
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Start_y1");
  }
  if(command == 'u'){
    pwms->setDuty(E_Y1, 0);
    pwms->setDuty(E_Y2, 200); 
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Start_y2");
  } 

  if(command == 'z'){
    pwms->setDuty(E_Z1, 200);
    pwms->setDuty(E_Z2, 0); 
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Start_z1");
  }
  if(command == 'a'){
    pwms->setDuty(E_Z1, 0);
    pwms->setDuty(E_Z2, 200); 
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Start_z2");
  }
  
  else if(command == 's'){
    pwms->setDuty(E_X1, 0);
    pwms->setDuty(E_X2, 0);
    pwms->setDuty(E_Y1, 0);
    pwms->setDuty(E_Y2, 0);
    pwms->setDuty(E_Z1, 0);
    pwms->setDuty(E_Z2, 0);
    Serial.print(timeNow);
    Serial.print(" - ");
    Serial.println("Stop");
  }

  
  else if(command == 'Y'){
    cnc->setYTarget(70, 20, true);  
  }
  
  else if(command == 'Z'){
    cnc->setZTarget(70, 20, true);  
  }
  
/*
 * Pas testé, mais ça pourrait être une façon de parser les commandes
 * Ex: CX 40, 30, 1, 0, 0 \n devrait donner cnc->setXTarget(40, 30, 1);
 * 
  while(!Serial.find('C'));
  E_CNC_COMMANDS command = (E_CNC_COMMANDS)Serial.read();
  long arg1 = Serial.parseInt();
  long arg2 = Serial.parseInt();
  long arg3 = Serial.parseInt();
  long arg4 = Serial.parseInt();
  long arg5 = Serial.parseInt();
  while(!Serial.find('\n'));
  
  switch(command){
    case E_STOP:
      cnc->stop();
      break;
    case E_SET_X_TARGET:
      cnc->setXTarget(arg1, arg2, (boolean)arg3);
      break;
    case E_SET_Y_TARGET:
      cnc->setXTarget(arg1, arg2, (boolean)arg3);
      break;
    case E_SET_Z_TARGET:
      cnc->setXTarget(arg1, arg2, (boolean)arg3);
      break;
    case E_SET_POSITION:
      cnc->setPosition(arg1, arg2, arg3);
      break;
    default:
      break;
  }*/
}
