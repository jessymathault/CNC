///////////////////////////////////////////////////////////
//
//    Factorial Pixel
//
//    Project : Design IV : CNC Machine
//    Author : Pierre-Olivier Cimon
//    Date : March 2016
//    Version : 1.0
//    Processor : Arduino Due
//    Description : Header             
//
///////////////////////////////////////////////////////////

#ifndef CNC_COMMUNICATION_H_
#define CNC_COMMUNICATION_H_

#include "Arduino.h"

#define INVALID 0 
#define VALID   1

#define DWELL     0
#define LINEAR    1
#define CIRCULAR  2
#define SETHOME   3

#define TIMEOUT   100  // How much time allowed to receive all bytes from one message.
#define MSG_LENGTH  24 // Number of bytes in one message.

class CNC_Communication
{
public:
  CNC_Communication(void);
  bool init(int _baudRate);
  bool readData(void);
  short get(void);
  char getData(uint8_t _dataIndex);

private:
 int baudRate;

 uint8_t nbByte;
 char buf[MSG_LENGTH];
 char header1, header2;
 char action;
 char mvtX_MSB, mvtX_LSB, mvtY_MSB, mvtY_LSB, mvtZ_MSB, mvtZ_LSB;
 char speedX_MSB, speedX_LSB, speedY_MSB, speedY_LSB, speedZ_MSB, speedZ_LSB; 
 
 char circleRadius_MSB, circleRadius_LSB;
 char angularSpeed_MSB, angularSpeed_LSB;
 char startAngle_MSB, startAngle_LSB;
 char endAngle_MSB, endAngle_LSB;
 char circleSense;

 short mvtX, mvtY, mvtZ;
 short speedX, speedY, speedZ;
 short circleRadius;
 short angularSpeed; 
 short startAngle;
 short endAngle;
  
};





#endif
