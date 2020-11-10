
#include "Arduino.h"
#include "CNC_Communication.h"

CNC_Communication::CNC_Communication(void)
{
  
}

bool CNC_Communication::init(int _baudRate)
{
  bool l_returnValue = INVALID;
  
  baudRate = _baudRate;
  if(baudRate == 300 || baudRate == 1200 || baudRate == 2400 || baudRate == 4800 || baudRate == 9600 ||
     baudRate == 19200 || baudRate == 38400 || baudRate == 576300 || baudRate == 748800 ||
     baudRate == 115200 || baudRate == 230400 || baudRate == 250000)
     {
        Serial.begin(baudRate);
        Serial.setTimeout(TIMEOUT); // How much time to receive all bytes from one message.
        l_returnValue = VALID;
     }

  return l_returnValue;
  
}//init(int _baudRate)

/*
 * Gets data from Raspberry Pi and assign values to variables.
 */
bool CNC_Communication::readData(void)
{  
  bool l_returnValue = 0;
  
  //Waiting for a message to come
  if (Serial.available() > 0)
  {
    nbByte = Serial.readBytesUntil(0x2A, buf, MSG_LENGTH+1); //Read the incoming data on serial bus and return the number of bytes read.

    //Empty buffer and variables assignement
    header1 = buf[0];
    header2 = buf[1];
    action = buf[2];
    mvtX_MSB = buf[3];
    mvtX_LSB = buf[4];
    mvtY_MSB = buf[5];
    mvtY_LSB = buf[6];
    mvtZ_MSB = buf[7];
    mvtZ_LSB = buf[8];
    speedX_MSB = buf[9];
    speedX_LSB = buf[10];
    speedY_MSB = buf[11];
    speedY_LSB = buf[12];
    speedZ_MSB = buf[13];
    speedZ_LSB  = buf[14];
    circleRadius_MSB = buf[15];
    circleRadius_LSB = buf[16];
    angularSpeed_MSB = buf[17];
    angularSpeed_LSB = buf[18];
    startAngle_MSB = buf[19];
    startAngle_LSB = buf[20];
    endAngle_MSB = buf[21];
    endAngle_LSB = buf[22];
    circleSense = buf[23];
    
    // Valid message received from Raspberry Pi
    if(0x7B == header1 && 0x7D == header2 && MSG_LENGTH == nbByte)
    {
      Serial.println("Valid message received !");
      l_returnValue = VALID; 

      if(DWELL == action)
      {
        //Wait
        Serial.println("Performing Dwell task");
      }
      else if(LINEAR == action)
      {
        mvtX = (mvtX_MSB << 8) + mvtX_LSB;
        mvtY = (mvtY_MSB << 8) + mvtY_LSB;
        mvtZ = (mvtZ_MSB << 8) + mvtZ_LSB;
        speedX = (speedX_MSB << 8) + speedX_LSB;
        speedY = (speedY_MSB << 8) + speedY_LSB;
        speedZ = (speedZ_MSB << 8) + speedZ_LSB;
      }
      else if(CIRCULAR == action)
      {
        circleRadius = (circleRadius_MSB << 8) + circleRadius_LSB;
        angularSpeed = (angularSpeed_MSB << 8) + angularSpeed_LSB;
        startAngle = (startAngle_MSB << 8) + startAngle_LSB;
        endAngle = (endAngle_MSB << 8) + endAngle_LSB;

        Serial.println("Performing Circular Movement");
        Serial.println("Parameters are :");
        Serial.print(" Radius : ");
        Serial.println(circleRadius, DEC);
        Serial.print(" Angular Speed : ");
        Serial.println(angularSpeed, DEC);
        Serial.print(" Start Angle : ");
        Serial.println(startAngle, DEC);
        Serial.print(" End Angle : ");
        Serial.println(endAngle, DEC);
        Serial.print(" Sense of rotation : ");
        Serial.println(circleSense, DEC);
      }
     else if(SETHOME == action)
      {
        // Calibration
        Serial.println("Recalibration...");
      }
    }// End valid message

    // Invalid message
    else
    {
       l_returnValue = INVALID;
       Serial.println("Invalid Message");
       if(0x7B != header1 || 0x7D != header2){Serial.println("Header incorrect...");}
       else if(MSG_LENGTH != nbByte){Serial.println("Msg Length incorrect...");Serial.println(nbByte, DEC);}
    }    
  }//End Serial available

  return l_returnValue;
}//readComm(void)

/*
 * Returns 16 bits parsed values.
 */
short CNC_Communication::get(uint8_t _index)
{
  short l_data;

  switch(_index)
  {
    case 0 : l_data = action; break;
    case 1 : l_data = mvtX; break;
    case 2 : l_data = mvtY; break;
    case 3 : l_data = mvtZ; break;
    case 4 : l_data = speedX; break;
    case 5 : l_data = speedY; break;
    case 6 : l_data = speedZ; break;
    case 7 : l_data = circleRadius; break; 
    case 8 : l_data = angularSpeed; break;
    case 9 : l_data = startAngle; break;
    case 10 : l_data = endAngle; break;
    case 11 : l_data = circleSense; break;          
    default : l_data = 0; break;    
  } 

  return l_data;
}

/*
 *  Returns raw data as received from serial. Mainly for debugging purpose.
 */
char CNC_Communication::getData(uint8_t _dataIndex)
{
  char l_data;

  switch(_dataIndex)
  {
    case 0 : l_data = action; break;
    case 1 : l_data = mvtX_MSB; break;
    case 2 : l_data = mvtX_LSB; break;
    case 3 : l_data = mvtY_MSB; break;
    case 4 : l_data = mvtY_LSB; break;
    case 5 : l_data = mvtZ_MSB; break;
    case 6 : l_data = mvtZ_LSB; break;
    case 7 : l_data = speedX_MSB; break;
    case 8 : l_data = speedX_LSB; break;
    case 9 : l_data = speedY_MSB; break;
    case 10 : l_data = speedY_LSB; break;
    case 11 : l_data = speedZ_MSB;  break;
    case 12 : l_data = speedZ_LSB; break;
    case 13 : l_data = circleRadius_MSB; break; 
    case 14 : l_data = circleRadius_LSB; break;
    case 15 : l_data = angularSpeed_MSB; break;
    case 16 : l_data = angularSpeed_LSB; break;
    case 17 : l_data = startAngle_MSB; break;
    case 18 : l_data = startAngle_LSB; break;
    case 19 : l_data = endAngle_MSB; break;
    case 20 : l_data = endAngle_LSB; break;
    case 21 : l_data = circleSense; break;          
    default : l_data = 0; break;    
  } 

  return l_data;
}
