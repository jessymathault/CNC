
#include "Arduino.h"
#include "CNC.h"
#include "Constants.h"

CNC::CNC(PWMs* pwms, MotorEncoders* encoders, LimitSwitches* switches, Leds* leds):
  pwms(pwms), encoders(encoders), switches(switches), leds(leds), currentState(E_IDLE), xTimeStamp(0), yTimeStamp(0), zTimeStamp(0), xTarget(0), yTarget(0), zTarget(0), xSpeed(vitesseX), ySpeed(vitesseY), zSpeed(vitesseZ){

  xSA = new ShiftingAverage();
  ySA = new ShiftingAverage();
  zSA = new ShiftingAverage();
  
  Xcontroller = new PID(XKp, XKi, XKd);
  Ycontroller = new PID(YKp, YKi, YKd);
  Zcontroller = new PID(ZKp, ZKi, ZKd);
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

void CNC::dwell(){
  return;
}

void CNC::lockedLoop(){ 
  long consigneFiltreX = 0;
  if (circularMotion == 0){
    consigneFiltreX = computeLinearSetPointX();
  }

  else{
    consigneFiltreX = computeCircularSetPointX();
  }
  //Serial.print(consigneFiltreX);
  //Serial.print("->");
  //Serial.print(encoders->getXPosition());
  //Serial.print(", ");
  Xcontroller->changeSetPoint(consigneFiltreX);
  long motorCommandX = Xcontroller->computeOutput(encoders->getXPosition(), 0);
  //Commande aux moteurs
  if(motorCommandX > 0){
    encoders->setXDirection(1);
    pwms->setDuty(E_X1, motorCommandX);
    pwms->setDuty(E_X2, 0);
  }
  else if(motorCommandX <= 0){
    encoders->setXDirection(2);
    pwms->setDuty(E_X1, 0);
    pwms->setDuty(E_X2, abs(motorCommandX));
  }

  long consigneFiltreY = 0;
  if (circularMotion == 0){
    consigneFiltreY = computeLinearSetPointY();
  }

  else{
    consigneFiltreY = computeCircularSetPointY();
  }
  //Consigne asservie en vitesse
  //Serial.print(consigneFiltreY);
  //Serial.print("->");
  //Serial.print(encoders->getYPosition());
  //Serial.print(", ");
  Ycontroller->changeSetPoint(consigneFiltreY);
  //Calcul de la sortie
  long motorCommandY = Ycontroller->computeOutput(encoders->getYPosition(), 0);
  //Commande aux moteurs
  if(motorCommandY > 0){
    encoders->setYDirection(1);
    pwms->setDuty(E_Y1, motorCommandY);
    pwms->setDuty(E_Y2, 0);
  }
  else if (motorCommandY <= 0){
    encoders->setYDirection(2);
    pwms->setDuty(E_Y1, 0);
    pwms->setDuty(E_Y2, abs(motorCommandY));
  }
  
  long consigneFiltreZ = 0;
  if (circularMotion == 0){
    consigneFiltreZ = computeLinearSetPointZ();
  }

  else{
    consigneFiltreZ = computeCircularSetPointZ();
  }
  //Consigne asservie en vitesse
  //Serial.print(consigneFiltreZ);
  //Serial.print("->");
  //Serial.print(encoders->getZPosition());
  //Serial.println(", ");
  Zcontroller->changeSetPoint(consigneFiltreZ);
  //Calcul de la sortie
  long motorCommandZ = Zcontroller->computeOutput(encoders->getZPosition(), 0);
  //Commande aux moteurs
  if(motorCommandZ > 0){
    encoders->setZDirection(1);
    pwms->setDuty(E_Z1, motorCommandZ);
    pwms->setDuty(E_Z2, 0);
  }
  else if (motorCommandZ <= 0){
    encoders->setZDirection(2);
    pwms->setDuty(E_Z1, 0);
    pwms->setDuty(E_Z2, abs(motorCommandZ));
  }

  //Est-ce qu'on est au bon endroit?
  if( encoders->getXPosition() == xTarget &&
      encoders->getYPosition() == yTarget &&
      encoders->getZPosition() == zTarget ){
    stop();
    setCurrentState(E_IDLE);
    Xcontroller->resetController();
    Ycontroller->resetController();
    Zcontroller->resetController();
  }
}

void CNC::golinear(long absPosX, float vx, long absPosY, float vy, long absPosZ, float vz){
  circularMotion = 0;
  setXTarget(absPosX, vx, true); 
  setYTarget(absPosY, vy, true); 
  setZTarget(absPosZ, vz, true); 
}


void CNC::setXTarget(long absolutePosition, float movementSpeed, boolean moveImmediately){
  initialX = encoders->getXPosition();
  xTimeStamp = millis();
  xTarget = absolutePosition;
  xSpeed = movementSpeed;
  
  Xcontroller->resetController();
  xSA->fill(encoders->getXPosition());
  
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
}

void CNC::setYTarget(long absolutePosition, float movementSpeed, boolean moveImmediately){
  initialY = encoders->getYPosition();
  yTimeStamp = millis();
  yTarget = absolutePosition;
  ySpeed = movementSpeed;
  
  Ycontroller->resetController();
  ySA->fill(encoders->getYPosition());
  
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
}

void CNC::setZTarget(long absolutePosition, float movementSpeed, boolean moveImmediately){
  initialZ = encoders->getZPosition();
  zTimeStamp = millis();
  zTarget = absolutePosition;
  zSpeed = movementSpeed;
  
  Zcontroller->resetController();
  zSA->fill(encoders->getZPosition());
  
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
}


long CNC::computeLinearSetPointX(){
  long consigneX = 0;
  if(xTarget < encoders->getXPosition()){
    consigneX = -1*xSpeed*(millis()-xTimeStamp)+initialX;
  }
  else{
    consigneX = xSpeed*(millis()-xTimeStamp)+initialX; 
  }
  
  //Moyenne sur n points
  xSA->addData(consigneX);
  long consigneFiltreX = xSA->getAverage();
  
  //Limite supérieure
  if(xTarget < encoders->getXPosition()){
    if(consigneFiltreX < xTarget){
      consigneFiltreX = xTarget;
    }
  }
  else{
    if(consigneFiltreX > xTarget){
      consigneFiltreX = xTarget;
    }
  }

  return consigneFiltreX;
}



long CNC::computeLinearSetPointY(){
  long consigneY = 0;
  if(yTarget < encoders->getYPosition()){
    consigneY = -1*ySpeed*(millis()-yTimeStamp)+initialY;
  }
  else{
    consigneY = ySpeed*(millis()-yTimeStamp)+initialY;
  }
  
  //Moyenne sur n points
  ySA->addData(consigneY);
  long consigneFiltreY = ySA->getAverage();
  
  //Limite supérieure
  if(yTarget < encoders->getYPosition()){
    if(consigneFiltreY < yTarget){
      consigneFiltreY = yTarget;
    }
  }
  else{
    if(consigneFiltreY > yTarget){
      consigneFiltreY = yTarget;
    }
  }
  return consigneFiltreY;
}

long CNC::computeLinearSetPointZ(){

  long consigneZ = 0;
  if(zTarget < encoders->getZPosition()){
    consigneZ = -1*zSpeed*(millis()-zTimeStamp)+initialZ;
  }
  else{
    consigneZ = zSpeed*(millis()-zTimeStamp)+initialZ;
  }
  
  //Moyenne sur n points
  zSA->addData(consigneZ);
  long consigneFiltreZ = zSA->getAverage();
  
  //Limite supérieure
  if(zTarget < encoders->getZPosition()){
    if(consigneFiltreZ < zTarget){
      consigneFiltreZ = zTarget;
    }
  }
  else{
    if(consigneFiltreZ > zTarget){
      consigneFiltreZ = zTarget;
    }
  }
  return consigneFiltreZ;
}

void CNC::goCircular(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW){
  circularMotion = 1;
  setXTargetCirc(theta0, thetaf, phi0, phif, r, movementSpeed, CCW, true);
  setYTargetCirc(theta0, thetaf, phi0, phif, r, movementSpeed, CCW, true);
  setZTargetCirc(theta0, thetaf, phi0, phif, r, movementSpeed, CCW, true);
}

void CNC::setXTargetCirc(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW, boolean moveImmediately){
  // Params entrée : Angles en degrés, radius en tics, vitesse en rad/ms
  // ***** theta0 < que thetaf si CCW
  // ***** theta0 > que thetaf si CW
  if (CCW){
    xSpeed = movementSpeed;
  }
  else {
    xSpeed = -1*movementSpeed;
  }
  
  initialX = encoders->getXPosition();
  xTimeStamp = millis();
  xTarget = round((r*cos((thetaf*3.1416/180))+initialX-r*cos((theta0*3.1416/180))));
  
  Xcontroller->resetController();
  xSA->fill(encoders->getXPosition());
  
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
  theta_0 = theta0*3.1416/180;
  theta_f = thetaf*3.1416/180;
  phi_0 = phi0*3.1416/180;
  phi_f = phif*3.1416/180;
  radius = r;
}

void CNC::setYTargetCirc(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW, boolean moveImmediately){
  // Params entrée : Angles en degrés, radius en tics, vitesse en rad/ms
  // ***** theta0 < que thetaf si CCW
  // ***** theta 0 > que thetaf si CW
  if (CCW){
    ySpeed = movementSpeed;
  }
  else {
    ySpeed = -1*movementSpeed;
  }
  
  initialY = encoders->getYPosition();
  yTimeStamp = millis();
  yTarget = round((r*sin((thetaf*3.1416/180))+initialY-r*sin((theta0*3.1416/180))));
  
  Ycontroller->resetController();
  ySA->fill(encoders->getYPosition());
  
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
  theta_0 = theta0*3.1416/180;
  theta_f = thetaf*3.1416/180;
  phi_0 = phi0*3.1416/180;
  phi_f = phif*3.1416/180;
  radius = r;
}
void CNC::setZTargetCirc(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW, boolean moveImmediately){
  // Params entrée : Angles en degrés, radius en tics, vitesse en rad/ms
  // ***** theta0 < que thetaf si CCW
  // ***** theta 0 > que thetaf si CW
  
  zSpeed = movementSpeed;
  
  initialZ = encoders->getZPosition();
  zTimeStamp = millis();
  zTarget = initialZ;
  
  Zcontroller->resetController();
  zSA->fill(encoders->getZPosition());
  
  if(moveImmediately){
    setCurrentState(E_BUSY);
  }  
}

long CNC::computeCircularSetPointX(){
  float currentAngle = xSpeed*(millis()-xTimeStamp)+theta_0; //rad
  long consigneX = 0;
  if ((currentAngle > theta_f)&&(xSpeed>0)){
    consigneX = round(radius*cos(theta_f) + initialX -radius*cos(theta_0));
  }
  else if ((currentAngle < theta_f)&&(xSpeed<0)){
    consigneX = round(radius*cos(theta_f) + initialX -radius*cos(theta_0));
  }
  else {
    consigneX = round(radius*cos(currentAngle)+initialX-radius*cos(theta_0));
  }

  //Moyenne sur n points
  xSA->addData(consigneX);
  long consigneFiltreX = xSA->getAverage();

  return consigneFiltreX;
}
long CNC::computeCircularSetPointY(){
  float currentAngle = ySpeed*(millis()-yTimeStamp)+theta_0; //rad
  long consigneY = 0;
  if ((currentAngle > theta_f)&&(ySpeed>0)){
    consigneY = round(radius*sin(theta_f) + initialY -radius*sin(theta_0));
  }
  else if ((currentAngle < theta_f)&&(ySpeed<0)){
    consigneY = round(radius*sin(theta_f) + initialY -radius*sin(theta_0));
  }
  else {
    consigneY = round(radius*sin(currentAngle)+initialY-radius*sin(theta_0));
  }

  //Moyenne sur n points
  ySA->addData(consigneY);
  long consigneFiltreY = ySA->getAverage();

  return consigneFiltreY;
}
long CNC::computeCircularSetPointZ(){
  return initialZ;
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


