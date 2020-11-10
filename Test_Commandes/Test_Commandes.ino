#include "CNC_Communication.h"

CNC_Communication uart;

int gcurrentTime = 0;
int gpreviousTime = 0;
int gdelay = 0;

short gaction, gmvtX_MSB, gmvtX_LSB, gmvtY_MSB, gmvtY_LSB, gmvtZ_MSB, gmvtZ_LSB, gspeedX_MSB, gspeedX_LSB, gspeedY_MSB,
      gspeedY_LSB, gspeedZ_MSB, gspeedZ_LSB, gcircleRadius_MSB, gcircleRadius_LSB, gangularSpeed_MSB, gangularSpeed_LSB, 
      gstartAngle_MSB, gstartAngle_LSB, gendAngle_MSB, gendAngle_LSB, gcircleSense;    

void setup() {
  // put your setup code here, to run once:
  uart.init(115200);
}//End Setup

void loop() {
  
  uart.readData(); //Lis les données en provenance du Rapsberry Pi
  
  gcurrentTime = millis();
  gdelay = abs(gcurrentTime - gpreviousTime);
  
  if(gdelay >= 3000)
  { 
      gpreviousTime = gcurrentTime;
      
      gaction = uart.getData(0);
      gmvtX_MSB = uart.getData(1);
      gmvtX_LSB = uart.getData(2);
      gmvtY_MSB = uart.getData(3);
      gmvtY_LSB = uart.getData(4);
      gmvtZ_MSB = uart.getData(5);
      gmvtZ_LSB = uart.getData(6);
      gspeedX_MSB = uart.getData(7);
      gspeedX_LSB = uart.getData(8);
      gspeedY_MSB = uart.getData(9);
      gspeedY_LSB = uart.getData(10);
      gspeedZ_MSB = uart.getData(11);
      gspeedZ_LSB = uart.getData(12);
      gcircleRadius_MSB = uart.getData(13);
      gcircleRadius_LSB = uart.getData(14);
      gangularSpeed_MSB = uart.getData(15);
      gangularSpeed_LSB  = uart.getData(16);
      gstartAngle_MSB = uart.getData(17);
      gstartAngle_LSB = uart.getData(18);
      gendAngle_MSB = uart.getData(19);
      gendAngle_LSB = uart.getData(20);
      gcircleSense = uart.getData(21);

      Serial.println(gaction, HEX);
      Serial.println(gmvtX_MSB, HEX);
      Serial.println(gmvtX_LSB, HEX);
      Serial.println(gmvtY_MSB, HEX);
      Serial.println(gmvtY_LSB, HEX);
      Serial.println(gmvtZ_MSB, HEX);
      Serial.println(gmvtZ_LSB, HEX);
      Serial.println(gspeedX_MSB, HEX);
      Serial.println(gspeedX_LSB, HEX);
      Serial.println(gspeedY_MSB, HEX);
      Serial.println(gspeedY_LSB, HEX);
      Serial.println(gspeedZ_MSB, HEX);
      Serial.println(gspeedZ_LSB, HEX);
      Serial.println(gcircleRadius_MSB, HEX);
      Serial.println(gcircleRadius_LSB, HEX);
      Serial.println(gangularSpeed_MSB, HEX);
      Serial.println(gangularSpeed_LSB, HEX); 
      Serial.println(gstartAngle_MSB, HEX); 
      Serial.println(gstartAngle_LSB, HEX); 
      Serial.println(gendAngle_MSB, HEX);
      Serial.println(gendAngle_LSB, HEX); 
      Serial.println(gcircleSense, HEX);
      Serial.println(gcircleSense, HEX);
      Serial.println("***********************************");
  }


}//End Loop
