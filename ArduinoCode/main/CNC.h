
#ifndef CNC_H_
#define CNC_H_

#include "Arduino.h"

#include "Leds.h"
#include "LimitSwitches.h"
#include "MotorEncoders.h"
#include "PWMs.h"
#include "PID.h"
#include "ShiftingAverage.h"

enum E_CNC_STATES{
  E_IDLE = 0,
  E_BUSY = 1,
  E_ERROR = 2  
};

class CNC{

public:
  CNC(PWMs* pwms, MotorEncoders* encoders, LimitSwitches* switches, Leds* leds);
  ~CNC();
  void init();

  void lockedLoop();

  void stop();
  void dwell();
  
  void setXTarget(long absolutePosition, float movementSpeed, boolean moveImmediately);
  void setYTarget(long absolutePosition, float movementSpeed, boolean moveImmediately);
  void setZTarget(long absolutePosition, float movementSpeed, boolean moveImmediately);

  void setXTargetCirc(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW, boolean moveImmediately);
  void setYTargetCirc(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW, boolean moveImmediately);
  void setZTargetCirc(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW, boolean moveImmediately);

  void golinear(long absPosX, float vx, long absPosY, float vy, long absPosZ, float vz); 

  void goCircular(float theta0, float thetaf, float phi0, float phif, float r, float movementSpeed, boolean CCW); 
  
  long computeLinearSetPointX();
  long computeLinearSetPointY();
  long computeLinearSetPointZ();

  long computeCircularSetPointX();
  long computeCircularSetPointY();
  long computeCircularSetPointZ();
  
   
  void setPosition(long xPosition, long yPosition, long zPosition);
  
  E_CNC_STATES getCurrentState();
  void setCurrentState(E_CNC_STATES newState);

public:
  MotorEncoders* encoders;
  Leds* leds;
  PWMs* pwms;
  LimitSwitches* switches;
  

  E_CNC_STATES currentState;

  PID* Xcontroller;
  PID* Ycontroller;
  PID* Zcontroller;

  ShiftingAverage* xSA;
  ShiftingAverage* ySA;
  ShiftingAverage* zSA;

  long xTimeStamp;
  long yTimeStamp;
  long zTimeStamp;

  long xTarget;
  long yTarget;
  long zTarget;

  float xSpeed;
  float ySpeed;
  float zSpeed;

  boolean circularMotion = 0;
  
  long initialX;
  long initialY;
  long initialZ;

  float theta_0;
  float theta_f;
  float phi_0;
  float phi_f;
  float radius;
};

#endif /* CNC_H_ */
