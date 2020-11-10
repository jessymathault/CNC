
#include "Arduino.h"
#include <assert.h>

#include "Constants.h"

#include "ManualEncoders.h"
#include "MotorEncoders.h"

//Périphériques
MotorEncoders* encoders = new MotorEncoders(43, 42, 41);
ManualEncoders* manualEncoders = new ManualEncoders(49, 50, 47, 46, 45, 44);

byte lastAction = 0;

void setup()
{
  Serial.begin(115200);
  encoders->init();
  manualEncoders->init();

  delay(1000);
}


void loop()
{
  if (lastAction == manualEncoders->debugX1())
    {}
  else
  {
    Serial.println(manualEncoders->debugX1());
    lastAction = manualEncoders->debugX1();
  }
  
}

