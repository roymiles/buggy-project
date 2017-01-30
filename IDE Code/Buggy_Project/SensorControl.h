/**
    SensorControl.h
    Purpose: This is class encapsulates all the grid sensor

    @author Roy Miles
    @version 1.0 11/12/2016
*/


#ifndef SensorControl_h
#define SensorControl_h

#include "Arduino.h"
#include "GridSensor.h"

#pragma once
class SensorControl
{
public:
  GridSensor *gs1; // Top left
  GridSensor *gs2; // Top right

  SensorControl();
  ~SensorControl();

};

#endif

