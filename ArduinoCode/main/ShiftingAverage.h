
#ifndef SHIFTING_AVERAGE_H_
#define SHIFTING_AVERAGE_H_

#include "Constants.h"

class ShiftingAverage{
  
public:
  ShiftingAverage();
  ~ShiftingAverage();
  
  void reset();
  long getAverage();
  void fill(long data);
  void addData(long point);

private:
  int i;
  long data[SA_NB_OF_POINTS];
};

#endif /* SHIFTING_AVERAGE_H_ */
