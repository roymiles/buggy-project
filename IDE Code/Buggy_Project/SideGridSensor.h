#ifndef SideGridSensor_h
#define SideGridSensor_h

#include "QTRSensors.h"
#include "Arduino.h" // Used for serial printing

#define NUM_SENSORS   2     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2 *NOT USED*

#pragma once
class SideGridSensor
{
public:
  SideGridSensor();
  ~SideGridSensor();

  unsigned int sideSensorValues[NUM_SENSORS];

  readSideSensors();

};

#endif
