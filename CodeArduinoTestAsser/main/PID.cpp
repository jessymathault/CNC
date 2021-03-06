
#include "Arduino.h"
#include "PID.h"

PID::PID(float Kp, float Ki, float Kd):
  Kp(Kp), Ki(Ki), Kd(Kd), setPoint(0), sumOfErrors(0), lastError(0){ 
}

PID::~PID(){
  
}

long PID::computeOutput(long currentPoint){
  long currentError = setPoint - currentPoint;
  long deltaError = currentError - lastError;
  sumOfErrors += currentError;
  lastError = currentError;
  
  return ((Kp*currentError) + (Ki*sumOfErrors) + (Kd*deltaError));
}

void PID::changeSetPoint(long newSetPoint){
  setPoint = newSetPoint;
}

long PID::getSetPoint(){
  return setPoint;
}

void PID::resetController(){
  sumOfErrors = 0;
  lastError = 0; 
}

