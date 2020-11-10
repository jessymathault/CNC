
#ifndef LIMITSWITCHES_H_
#define LIMITSWITCHES_H_

#include "Arduino.h"

class LimitSwitches{

public:
  LimitSwitches(int pinX1, int pinX2, int pinX3, int pinX4, int pinY1, int pinY2, int pinY3, int pinY4, int pinZ1, int pinZ2, int pinZ3, int pinZ4);
  ~LimitSwitches();
  void init();

  boolean anyIsPressed();

private:
  int pinX1;
  int pinX2;
  int pinX3;
  int pinX4;
  int pinY1;
  int pinY2;
  int pinY3;
  int pinY4;
  int pinZ1;
  int pinZ2;
  int pinZ3;
  int pinZ4;
};

#endif /* LIMITSWITCHES_H_ */
