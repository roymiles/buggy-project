#ifndef SideGridSensor_h
#define SideGridSensor_h

#include "QTRSensors.h"
#include "Arduino.h" // Used for serial printing

#define SIDE_NUM_SENSORS   2     // number of sensors used
#define SIDE_TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define SIDE_EMITTER_PIN   13    // emitter is controlled by digital pin 2 *NOT USED*

#pragma once
class SideGridSensor
{
public:
  SideGridSensor();
  ~SideGridSensor();
  
  unsigned int getMinimum(unsigned int index);
  unsigned int getMaximum(unsigned int index);  

  unsigned int sideSensorValues[SIDE_NUM_SENSORS];

  void readSideSensors();
  void calibrate();
  void postCalibration();

};

#endif
