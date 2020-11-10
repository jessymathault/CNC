
#include "Arduino.h"
#include "LimitSwitches.h"


volatile boolean XleftHit = false;
volatile boolean XrightHit = false;
volatile boolean YtopHit = false;
volatile boolean YbottomHit = false;
volatile boolean ZupHit = false;
volatile boolean ZdownHit = false;


//On vérifie l'état des pins mais y faudrait peut-être un mini-debounce
void XleftISR(void){
  if(digitalRead(29) == LOW || digitalRead(30) == LOW){
    XleftHit = true;  
  }
  else{
    XleftHit = false;
  }
}

void XrightISR(void){
  if(digitalRead(31) == LOW || digitalRead(32) == LOW){
    XrightHit = true;  
  }
  else{
    XrightHit = false;
  }
}

void YtopISR(void){
  if(digitalRead(33) == LOW || digitalRead(34) == LOW){
    YtopHit = true;  
  }
  else{
    YtopHit = false;
  }
}

void YbottomISR(void){
  if(digitalRead(35) == LOW || digitalRead(36) == LOW){
    YbottomHit = true;  
  }
  else{
    YbottomHit = false;
  }
}

void ZupISR(void){
  if(digitalRead(37) == LOW || digitalRead(38) == LOW){
    ZupHit = true;  
  }
  else{
    ZupHit = false;
  }
}

void ZdownISR(void){
  if(digitalRead(39) == LOW || digitalRead(40) == LOW){
    ZdownHit = true;  
  }
  else{
    ZdownHit = false;
  }
}


LimitSwitches::LimitSwitches(int pinX1, int pinX2, int pinX3, int pinX4, int pinY1, int pinY2, int pinY3, int pinY4, int pinZ1, int pinZ2, int pinZ3, int pinZ4):
  pinX1(pinX1), pinX2(pinX2), pinX3(pinX3), pinX4(pinX4), pinY1(pinY1), pinY2(pinY2), pinY3(pinY3), pinY4(pinY4), pinZ1(pinZ1), pinZ2(pinZ2), pinZ3(pinZ3), pinZ4(pinZ4){  
}

LimitSwitches::~LimitSwitches(){
 
}

void LimitSwitches::init(){  
  pinMode(pinX1, INPUT_PULLUP);
  attachInterrupt(pinX1, XleftISR, CHANGE);
  pinMode(pinX2, INPUT_PULLUP);
  attachInterrupt(pinX2, XleftISR, CHANGE);
  pinMode(pinX3, INPUT_PULLUP);
  attachInterrupt(pinX3, XrightISR, CHANGE);
  pinMode(pinX4, INPUT_PULLUP);
  attachInterrupt(pinX4, XrightISR, CHANGE);
  
  pinMode(pinY1, INPUT_PULLUP);
  attachInterrupt(pinY1, YtopISR, CHANGE);
  pinMode(pinY2, INPUT_PULLUP);
  attachInterrupt(pinY2, YtopISR, CHANGE);
  pinMode(pinY3, INPUT_PULLUP);
  attachInterrupt(pinY3, YbottomISR, CHANGE);
  pinMode(pinY4, INPUT_PULLUP);
  attachInterrupt(pinY4, YbottomISR, CHANGE);
  
  pinMode(pinZ1, INPUT_PULLUP);
  attachInterrupt(pinZ1, ZupISR, CHANGE);
  pinMode(pinZ2, INPUT_PULLUP);
  attachInterrupt(pinZ2, ZupISR, CHANGE); 
  pinMode(pinZ3, INPUT_PULLUP);
  attachInterrupt(pinZ3, ZdownISR, CHANGE);
  pinMode(pinZ4, INPUT_PULLUP);
  attachInterrupt(pinZ4, ZdownISR, CHANGE);  
}

boolean LimitSwitches::anyIsPressed(){
  return XleftHit || XrightHit || YtopHit || YbottomHit || ZupHit || ZdownHit;
}

