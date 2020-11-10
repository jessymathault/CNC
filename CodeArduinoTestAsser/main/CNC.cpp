
#include "Arduino.h"
#include "CNC.h"

CNC::CNC(PWMs* pwms, MotorEncoders* encoders, LimitSwitches* switches, Leds* leds):
  pwms(pwms), encoders(encoders), switches(switches), leds(leds), currentState(E_BUSY){

  //TODO Trouve0 les constantes d'asservissement  
  Xcontroller = new PID(2, 0, 0);
  Ycontroller = new PID(2, 0, 0);
  Zcontroller = new PID(2, 0, 0);
}

CNC::~CNC(){
  
}

void CNC::init(){
  //Pins a utiliser pour la logique des protections
  //À utiliser plus tard
  pinMode(24, OUTPUT); //Z drive reset
  digitalWrite(24, HIGH);

  pinMode(27, OUTPUT); //Y drive reset
  digitalWrite(27, HIGH);
  
  pinMode(28, OUTPUT); //X drive reset
  digitalWrite(28, HIGH);

  pinMode(53, OUTPUT); //ToolEnable

  pinMode(22, INPUT); //Temp warning Z
  pinMode(23, INPUT); //Temp fault Z

  pinMode(25, INPUT); //Temp warning XY
  pinMode(26, INPUT); //Temp fault XY

  pinMode(A0, INPUT); //Red button 0 = enfoncé

  pinMode(A1, INPUT); //Motor current
  pinMode(A2, INPUT); //Motor current
  pinMode(A3, INPUT); //Motor current

  pinMode(A8, INPUT); //Manual button push speed
  pinMode(A9, INPUT); //Manual button push speed
  pinMode(A10, INPUT); //Manual button push speed 
}

void CNC::stop(){
  pwms->setDuty(E_X1, 0);
  pwms->setDuty(E_X2, 0);
  pwms->setDuty(E_Y1, 0);
  pwms->setDuty(E_Y2, 0);
  pwms->setDuty(E_Z1, 0);
  pwms->setDuty(E_Z2, 0);
}

void CNC::lockedLoop(){
  if(encoders->getXPosition() == Xcontroller->getSetPoint()){ //TODO est-ce qu'on se laisse 1 delta d'incertitude?
    pwms->setDuty(E_X1, 0);
    pwms->setDuty(E_X2, 0);
  }

  if(encoders->getYPosition() == Ycontroller->getSetPoint()){ //TODO est-ce qu'on se laisse 1 delta d'incertitude?
    pwms->setDuty(E_Y1, 0);
    pwms->setDuty(E_Y2, 0);
  }

  if(encoders->getZPosition() == Zcontroller->getSetPoint()){ //TODO est-ce qu'on se laisse 1 delta d'incertitude?
    pwms->setDuty(E_Z1, 0);
    pwms->setDuty(E_Z2, 0);
  }

  //Non? On asservie!
  long motorCommandX = Xcontroller->computeOutput(encoders->getXPosition());
  if(motorCommandX > 0){
    pwms->setDuty(E_X1, motorCommandX);
    pwms->setDuty(E_X2, 0);
  }
  else{
    pwms->setDuty(E_X1, 0);
    pwms->setDuty(E_X2, motorCommandX);
  }

  long motorCommandY = Ycontroller->computeOutput(encoders->getYPosition());
  if(motorCommandY > 0){
    pwms->setDuty(E_Y1, motorCommandY);
    pwms->setDuty(E_Y2, 0);
  }
  else{
    pwms->setDuty(E_Y1, 0);
    pwms->setDuty(E_Y2, motorCommandY);
  }

  long motorCommandZ = Zcontroller->computeOutput(encoders->getZPosition());
  if(motorCommandZ > 0){
    pwms->setDuty(E_Z1, motorCommandZ);
    pwms->setDuty(E_Z2, 0);
  }
  else{
    pwms->setDuty(E_Z1, 0);
    pwms->setDuty(E_Z2, motorCommandZ);
  }

  //Est-ce qu'on est au bon endroit?
  if( encoders->getXPosition() == Xcontroller->getSetPoint() &&
      encoders->getYPosition() == Ycontroller->getSetPoint() &&
      encoders->getZPosition() == Zcontroller->getSetPoint() ){
    setCurrentState(E_IDLE);
  }
}

void CNC::setXTarget(long absolutePosition, long movementSpeed, boolean moveImmediately){
  Xcontroller->changeSetPoint(absolutePosition);
  //TODO Configurer la vitesse
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }
}

void CNC::setYTarget(long absolutePosition, long movementSpeed, boolean moveImmediately){
  Ycontroller->changeSetPoint(absolutePosition);
  //TODO Configurer la vitesse
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
}

void CNC::setZTarget(long absolutePosition, long movementSpeed, boolean moveImmediately){
  Zcontroller->changeSetPoint(absolutePosition);
  //TODO Configurer la vitesse
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }
}

void CNC::setPosition(long xPosition, long yPosition, long zPosition){
  encoders->setXPosition(xPosition);
  encoders->setYPosition(yPosition);
  encoders->setZPosition(zPosition);  
}

E_CNC_STATES CNC::getCurrentState(){
  return currentState;
}

void CNC::setCurrentState(E_CNC_STATES newState){
  currentState = newState;
}


