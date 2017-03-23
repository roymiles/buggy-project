#ifndef FrontGridSensor_h
#define FrontGridSensor_h

#include "QTRSensors.h"
#include "Arduino.h" // Used for serial printing

#pragma once

#define FRONT_NUM_SENSORS   3     // number of sensors used
#define FRONT_TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define FRONT_EMITTER_PIN   12    // emitter is controlled by digital pin 2

class FrontGridSensor
{
public:
  FrontGridSensor();
  ~FrontGridSensor();

  unsigned int frontSensorValues[FRONT_NUM_SENSORS];

  void readFrontSensors();
  void calibrate();
  void postCalibration();

  unsigned int getMinimum(unsigned int index);
  unsigned int getMaximum(unsigned int index);

};

#endif
