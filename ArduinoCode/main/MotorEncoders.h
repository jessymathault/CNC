
#ifndef MOTORENCODERS_H_
#define MOTORENCODERS_H_

class MotorEncoders{

public:
  MotorEncoders(int encoderX1Pin, int encoderY1Pin, int encoderZ1Pin);
  ~MotorEncoders();
  void init();

  long getXPosition();
  long setXPosition(long positionInTicks);
  
  long getYPosition();
  long setYPosition(long positionInTicks);
  
  long getZPosition();
  long setZPosition(long positionInTicks);

  void setXDirection(byte positive);
  void setYDirection(byte positive);
  void setZDirection(byte positive);

private:
  int encoderX1Pin;
  int encoderX2Pin;
  int encoderY1Pin;
  int encoderY2Pin;
  int encoderZ1Pin;
  int encoderZ2Pin;
};

#endif /* MOTORENCODERS_H_ */
