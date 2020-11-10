
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
  long getXCounter(void);
  long getYCounter(void);
  long getZCounter(void);

  long getlastXCounter(void);
  long getlastYCounter(void);
  long getlastZCounter(void);

  void resetXCounter(void);
  void resetYCounter(void);
  void resetZCounter(void);
  
private:
  int pinX1;
  int pinX2;
  int pinY1;
  int pinY2;
  int pinZ1;
  int pinZ2;
};

#endif /* MANUALENCODERS_H_ */
