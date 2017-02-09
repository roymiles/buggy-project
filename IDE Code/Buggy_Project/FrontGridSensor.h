#ifndef FrontGridSensor_h
#define FrontGridSensor_h

#include "QTRSensors.h"
#include "Arduino.h" // Used for serial printing

#define FRONT_NUM_SENSORS   2     // number of sensors used
#define FRONT_TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define FRONT_EMITTER_PIN   2     // emitter is controlled by digital pin 2 *NOT USED*

#pragma once
class FrontGridSensor
{
public:
  FrontGridSensor();
  ~FrontGridSensor();

  unsigned int frontSensorValues[FRONT_NUM_SENSORS];

  readFrontSensors();

};

#endif
