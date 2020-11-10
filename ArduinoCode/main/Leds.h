
#ifndef LEDS_H_
#define LEDS_H_

#include "Arduino.h"

class Leds{

public:
	Leds(int redPin, int yellowPin, int greenPin);
	~Leds();
  void init();

  void setRedLed(bool state);
  void setYellowLed(bool state);
  void setGreenLed(bool state);

  void turnAllOff();
  void turnAllOn();

private:
  int redPin;
  int yellowPin;
  int greenPin;
};

#endif /* LEDS_H_ */
