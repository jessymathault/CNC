
#include "ShiftingAverage.h"
#include <Arduino.h>
#include <string.h>

ShiftingAverage::ShiftingAverage():
  i(0){
  memset(data, 0, sizeof(data));
}
  
ShiftingAverage::~ShiftingAverage(){
  
}
  
void ShiftingAverage::reset(){
  i = 0;
  for(int i = 0; i < SA_NB_OF_POINTS ; ++i){
    data[i] = 0;  
  }
}

long ShiftingAverage::getAverage(){
  long sum = 0;
  for(int i = 0; i < SA_NB_OF_POINTS ; ++i){
    sum += data[i];  
  }
  long result = sum/SA_NB_OF_POINTS;
  //Serial.println(result);
  return sum/SA_NB_OF_POINTS;
}

void ShiftingAverage::fill(long d){
  for(int i = 0; i < SA_NB_OF_POINTS ; ++i){
    data[i] = d;  
  }
}

void ShiftingAverage::addData(long point){
  data[i] = point;
  
  if(i < (SA_NB_OF_POINTS-1)){
    i++;  
  }
  else{
    i = 0;
  }
}

