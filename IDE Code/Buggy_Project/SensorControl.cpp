/**
    SensorControl.cpp
    Purpose: This is class encapsulates all the grid sensor

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "SensorControl.h"


SensorControl::SensorControl()
{
  // Create instances of the grid sensors for each position
  gs1  = new GridSensor(sensorPosition::TOP_LEFT);
  gs2  = new GridSensor(sensorPosition::TOP_RIGHT);
}


SensorControl::~SensorControl()
{
}


