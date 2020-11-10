
#ifndef PID_H_
#define PID_H_

#include "Arduino.h"

class PID{

public:
  PID(float Kp, float Ki, float Kd);
  ~PID();

  long computeOutput(long processOutput, long deltaT);
  
  void changeSetPoint(long newSetPoint);
  long getSetPoint();

  void resetController();

public:
  float Kp;
  float Ki;
  float Kd;
  
  long setPoint;

  long sumOfErrors;
  long lastError;
};

#endif /* PID_H_ */
