
#ifndef MANUALENCODERS_H_
#define MANUALENCODERS_H_

#include "Arduino.h"

class ManualEncoders{

public:
  ManualEncoders(int pinX1, int pinX2, int pinY1, int pinY2, int pinZ1, int pinZ2);
  ~ManualEncoders();
  void init();

private:
  int pinX1;
  int pinX2;
  int pinY1;
  int pinY2;
  int pinZ1;
  int pinZ2;
};

#endif /* MANUALENCODERS_H_ */
