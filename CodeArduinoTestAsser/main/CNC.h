
#ifndef CNC_H_
#define CNC_H_

#include "Arduino.h"

#include "Leds.h"
#include "LimitSwitches.h"
#include "MotorEncoders.h"
#include "PWMs.h"
#include "PID.h"

enum E_CNC_STATES{
  E_IDLE,
  E_BUSY,
  E_ERROR  
};

class CNC{

public:
  CNC(PWMs* pwms, MotorEncoders* encoders, LimitSwitches* switches, Leds* leds);
  ~CNC();
  void init();

  void lockedLoop();

  void stop();
  
  void setXTarget(long absolutePosition, long movementSpeed, boolean moveImmediately);
  void setYTarget(long absolutePosition, long movementSpeed, boolean moveImmediately);
  void setZTarget(long absolutePosition, long movementSpeed, boolean moveImmediately);

  void setPosition(long xPosition, long yPosition, long zPosition);
  
  E_CNC_STATES getCurrentState();
  void setCurrentState(E_CNC_STATES newState);

  MotorEncoders* encoders;
  
private:
  PWMs* pwms;
  LimitSwitches* switches;
  Leds* leds;

  E_CNC_STATES currentState;

  PID* Xcontroller;
  PID* Ycontroller;
  PID* Zcontroller;
};

#endif /* CNC_H_ */
