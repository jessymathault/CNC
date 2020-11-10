// rechanger le baudrate à 115200
#include "Arduino.h"
#include <assert.h>
#include <math.h>

#include "Constants.h"

#include "CNC.h"
#include "Leds.h"
#include "LimitSwitches.h"
#include "ManualEncoders.h"
#include "MotorEncoders.h"
#include "PID.h"
#include "PWMs.h"
#include "CNC_Communication.h"

//Périphériques
Leds* leds = new Leds(51, 50, 52);
LimitSwitches* switches = new LimitSwitches(29, 30, 31, 32, 33, 34, 35, 36, 37, 40, 38, 39); //ORDRE IMPORTANT
PWMs* pwms = new PWMs(7, 6, 9, 8, 4, 5);
MotorEncoders* encoders = new MotorEncoders(43, 42, 41);
ManualEncoders* manualEncoders = new ManualEncoders(44, 45, 46, 47, 48, 49);
CNC_Communication uart;

//La classe CNC agit comme controller
CNC* cnc = new CNC(pwms, encoders, switches, leds);

boolean everythingIsOk = true;

#define boutonRouge A0
long boutonCheck = 0;
boolean boutonOk = true;
boolean boutonHadProblem = false;

volatile boolean l;
//TC1 ch 0
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  digitalWrite(13, l = !l);
  Serial.print(cnc->encoders->getXPosition());
  Serial.print(" ");
  Serial.print(cnc->encoders->getYPosition());
  Serial.print(" ");
  Serial.print(cnc->encoders->getZPosition());
  Serial.print(" ");
  Serial.print(cnc->getCurrentState());
  Serial.print(" ");
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
}

void setup()
{
  uart.init(115200);

  leds->init();
  switches->init();
  encoders->init();
  manualEncoders->init();
  pwms->init();
  cnc->init();
  
  startTimer(TC1, 0, TC3_IRQn, 5); //TC1 channel 0, the IRQ for that channel and the desired frequency
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(boutonRouge, INPUT);
}

void loop()
{
  if ((switches->anyIsPressed())&&(everythingIsOk)) {
    cnc->setCurrentState(E_ERROR);
  }
  if ((millis() - boutonCheck) > 1000) {
    boutonOk = (analogRead(boutonRouge)>400);
  }
  if (boutonOk == false){
    cnc->setCurrentState(E_ERROR);
    boutonHadProblem = true;
  }

  if ((boutonOk)&&(boutonHadProblem == true)){
    cnc->setCurrentState(E_IDLE);
    boutonHadProblem = false;
    leds->setRedLed(false);
  }

  if ((E_CNC_STATES)cnc->getCurrentState() == E_ERROR) {
    everythingIsOk = false;
    leds->setRedLed(true);
    cnc->stop();
    cnc->setXTarget(encoders->getXPosition(), 1, true);
    cnc->setYTarget(encoders->getYPosition(), 1, true);
    cnc->setZTarget(encoders->getZPosition(), 1, true);
    
    if (switches->pxLimit()){
      cnc->setXTarget((encoders->getXPosition()+40), 0.05, true);
    }
    else if (switches->nxLimit()){
      cnc->setXTarget((encoders->getXPosition()-40), 0.05, true);
    }
    else if (switches->pyLimit()){
      cnc->setYTarget((encoders->getYPosition()+40), 0.05, true);
    }
    else if (switches->nyLimit()){
      cnc->setYTarget((encoders->getYPosition()-40), 0.05, true);
    }
    else if (switches-> pzLimit()){
      cnc->setZTarget((encoders->getZPosition()-40), 0.05, true);
    }
    else if (switches->nzLimit()){
      cnc->setZTarget((encoders->getZPosition()+40), 0.05, true);
    }
    
    return;
  }
  
  else if ((E_CNC_STATES)cnc->getCurrentState() == E_BUSY) {
    leds->setYellowLed(true);
    leds->setGreenLed(false);
    cnc->lockedLoop();

  }
  
  else if ((E_CNC_STATES)cnc->getCurrentState() == E_IDLE) {
    leds->setYellowLed(false);
    leds->setGreenLed(true);
    everythingIsOk = true;
    encoders->setXDirection(0);
    encoders->setYDirection(0);
    encoders->setZDirection(0);

    // manual control
    long goalx = encoders->getXPosition();
    long goaly = encoders->getYPosition();
    long goalz = encoders->getZPosition();
    
    long tempx = manualEncoders->getlastXCounter();
    if (manualEncoders->getXCounter() != tempx){
      goalx += (manualEncoders->getXCounter()-tempx)*10;
      cnc->setXTarget(goalx, 1, true);
    }
    
    long tempy = manualEncoders->getlastYCounter();
    if (manualEncoders->getYCounter() != tempy){
      goaly += (manualEncoders->getYCounter()-tempy)*10;
      cnc->setYTarget(goaly, 1, true);
    }

    long tempz = manualEncoders->getlastZCounter();
    if (manualEncoders->getZCounter() != tempz){
      goalz += (manualEncoders->getZCounter()-tempz)*5;
      cnc->setZTarget(goalz, 1, true);
    }

    
  }
  
  else{
    cnc->stop();
    assert(0); //Unknown state
  }
}

long Xpos = 0;
long Ypos = 0;
long Zpos = 0;
  
//Appelé à chaque loop() si le buffer du UART a des données
void serialEvent() {
  //Get command
  int header = Serial.parseInt();
  if(header != 255){ 
    cnc->stop();   
  }

  long action = Serial.parseInt();

  long xf = Serial.parseInt();
  long yf = Serial.parseInt();
  long zf = Serial.parseInt();  

  float vt = Serial.parseFloat();

  float r = Serial.parseFloat(); 
  float thetaI = Serial.parseFloat();
  float thetaF = Serial.parseFloat();

  long milli = Serial.parseInt();

  int footer = Serial.parseInt();
  if(footer != 254){
    cnc->stop();
  }

  //Parse command
  if (action == -1){
    cnc->stop();
  }
  else if(action == 0 || action == 28){
    cnc->golinear(xf, 0.2, yf, 0.2, zf, 0.2); 
  }
  else if(action == 1){
    long xi = encoders->getXPosition();
    long yi = encoders->getYPosition();
    long zi = encoders->getZPosition();

    float tTot = (sqrt(sq(xf-xi)+sq(zf-zi)+sq(yf-yi)))/vt;
    
    float vx = abs((xf-xi)/tTot);
    float vy = abs((yf-yi)/tTot);
    float vz = abs((zf-zi)/tTot);
    
    cnc->golinear(xf, vx, yf, vy, zf, vz); 
  }
  else if(action == 2){
    //assert(thetaF > thetaI);
    
    float vAng = vt/(r);
    cnc->goCircular(thetaI, thetaF, 90, 90, r, vAng, false);    
  }
  else if(action == 3){
    //assert(thetaF < thetaI);
    
    float vAng = vt/(r);
    cnc->goCircular(thetaI, thetaF, 90, 90, r, vAng, true);
  }
  else if(action == 4){
    //Serial.println("Dwell unsupported atm");
  }
  else if(action == 92){
    cnc->setPosition(xf, yf, zf);
  }
}
