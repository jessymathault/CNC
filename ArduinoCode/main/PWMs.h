
#ifndef PWMs_H_
#define PWMs_H_

enum E_CHANNEL{
  E_X1,
  E_X2,
  E_Y1,
  E_Y2,
  E_Z1,
  E_Z2
};

class PWMs{
  
public:
  PWMs(int pinX1, int pinX2, int pinY1, int pinY2, int pinZ1, int pinZ2);
  ~PWMs();
  void init();

  void setDuty(E_CHANNEL channel, int ticks);

private:
  int pinX1;
  int pinX2;
  int pinY1;
  int pinY2;
  int pinZ1;
  int pinZ2;
};

#endif /* PWMs_H_ */
