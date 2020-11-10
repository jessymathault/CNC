//Serial.println("L");
//TODO il va falloir détecter la phase avec des variables volatiles qui toggle
// Pour déterminer la direction

//Chaque ISR doit modifier la consigne selon un ratio ça va donner une position selon le nombre de tours

//On va faire ce mode de contrôle à la fin surement

#include "Arduino.h"
#include "ManualEncoders.h"

long lastXasked = 0;
long lastYasked = 0;
long lastZasked = 0;

volatile byte senseRotationX = 0;
volatile byte senseRotationY = 0;
volatile byte senseRotationZ = 0;
volatile long xCounter = 0;
volatile long yCounter = 0;
volatile long zCounter = 0;

volatile long lastDebounceXLeft = 0;  // the last time the output pin was toggled
volatile long lastDebounceXRight = 0;
volatile long lastDebounceYLeft = 0;
volatile long lastDebounceYRight = 0;
volatile long lastDebounceZLeft = 0;
volatile long lastDebounceZRight = 0;
long debounceDelay = 10;    // the debounce time; increase if the output flickers

void xLeftISR(void) {

  if ((millis() - lastDebounceXLeft) > debounceDelay) {
    if ((digitalRead(44) == LOW) && (digitalRead(45) == LOW) && (senseRotationX == 2))
    {
      xCounter++;
    }
    else if ((digitalRead(44) == LOW) && (digitalRead(45) == HIGH))
    {
      senseRotationX = 1;
    }
    else
    {
      senseRotationX = 0;
    }
    lastDebounceXLeft = millis();
  }


}

void xRightISR(void) {

  if ((millis() - lastDebounceXRight) > debounceDelay)
  {
    if ((digitalRead(44) == LOW) && (digitalRead(45) == LOW) && (senseRotationX == 1))
    {
      xCounter--;
    }
    else if ((digitalRead(44) == HIGH) && (digitalRead(45) == LOW))
    {
      senseRotationX = 2;
    }
    else
    {
      senseRotationX = 0;
    }
    lastDebounceXRight = millis();
  }
}


void yLeftISR(void) {
  if ((millis() - lastDebounceYLeft) > debounceDelay) {

    if ((digitalRead(46) == LOW) && (digitalRead(47) == LOW) && (senseRotationY == 2))
    {
      yCounter++;
    }
    else if ((digitalRead(46) == LOW) && (digitalRead(47) == HIGH))
    {
      senseRotationY = 1;
    }
    else
    {
      senseRotationY = 0;
    }
    lastDebounceYLeft = millis();
  }
}

void yRightISR(void) {
  if ((millis() - lastDebounceYRight) > debounceDelay) {
    if ((digitalRead(46) == LOW) && (digitalRead(47) == LOW) && (senseRotationY == 1))
    {
      yCounter--;
    }
    else if ((digitalRead(46) == HIGH) && (digitalRead(47) == LOW))
    {
      senseRotationY = 2;
    }
    else
    {
      senseRotationY = 0;
    }
    lastDebounceYRight = millis();
  }
}

void zLeftISR(void) {
  if ((millis() - lastDebounceZLeft) > debounceDelay) {
    if ((digitalRead(48) == LOW) && (digitalRead(49) == LOW) && (senseRotationZ == 2))
    {
      zCounter++;
    }
    else if ((digitalRead(48) == LOW) && (digitalRead(49) == HIGH))
    {
      senseRotationZ = 1;
    }
    else
    {
      senseRotationZ = 0;
    }
    lastDebounceZLeft = millis();
  }
}

void zRightISR(void) {
  if ((millis() - lastDebounceZRight) > debounceDelay) {
    if ((digitalRead(48) == LOW) && (digitalRead(49) == LOW) && (senseRotationZ == 1))
    {
      zCounter--;
    }
    else if ((digitalRead(48) == HIGH) && (digitalRead(49) == LOW))
    {
      senseRotationZ = 2;
    }
    else
    {
      senseRotationZ = 0;
    }
    lastDebounceZRight = millis();
  }
}

ManualEncoders::ManualEncoders(int pinX1, int pinX2, int pinY1, int pinY2, int pinZ1, int pinZ2):
  pinX1(pinX1), pinX2(pinX2), pinY1(pinY1), pinY2(pinY2), pinZ1(pinZ1), pinZ2(pinZ2) {
}

ManualEncoders::~ManualEncoders() {

}

void ManualEncoders::init() {
  pinMode(pinX1, INPUT);
  attachInterrupt(pinX1, xLeftISR, FALLING);

  pinMode(pinX2, INPUT);
  attachInterrupt(pinX2, xRightISR, FALLING);

  pinMode(pinY1, INPUT);
  attachInterrupt(pinY1, yLeftISR, FALLING);

  pinMode(pinY2, INPUT);
  attachInterrupt(pinY2, yRightISR, FALLING);

  pinMode(pinZ1, INPUT);
  attachInterrupt(pinZ1, zLeftISR, FALLING);

  pinMode(pinZ2, INPUT);
  attachInterrupt(pinZ2, zRightISR, FALLING);

  long lastXasked = 0;
  long lastYasked = 0;
  long lastZasked = 0;

  volatile long xCounter = 0;
  volatile long yCounter = 0;
  volatile long zCounter = 0;
}

long ManualEncoders::getXCounter(void)
{
  return xCounter;
}

long ManualEncoders::getYCounter(void)
{
  return yCounter;
}
long ManualEncoders::getZCounter(void)
{
  return zCounter;
}

void ManualEncoders::resetXCounter(void)
{
  xCounter = 0;
}
void ManualEncoders::resetYCounter(void)
{
  yCounter = 0;
}
void ManualEncoders::resetZCounter(void)
{
  zCounter = 0;
}

long ManualEncoders::getlastXCounter(void){
  long temp = lastXasked;
  lastXasked = xCounter;
  return temp;
}
long ManualEncoders::getlastYCounter(void){
  long temp = lastYasked;
  lastYasked = yCounter;
  return temp;
}
long ManualEncoders::getlastZCounter(void){
  long temp = lastZasked;
  lastZasked = zCounter;
  return temp;
}

