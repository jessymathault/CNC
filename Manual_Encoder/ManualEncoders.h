
#ifndef MANUALENCODERS_H_
#define MANUALENCODERS_H_

#include "Arduino.h"

#define CW    1   //Clockwise
#define CCW   2   //Counter Clockwise

class ManualEncoders{

public:
  ManualEncoders(int pinX1, int pinX2, int pinY1, int pinY2, int pinZ1, int pinZ2);
  ~ManualEncoders();
  void init();
  int debugX1(void);
  int debugX2(void);
  
private:
  int pinX1;
  int pinX2;
  int pinY1;
  int pinY2;
  int pinZ1;
  int pinZ2;
};

#endif /* MANUALENCODERS_H_ */
