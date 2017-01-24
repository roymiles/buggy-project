/**
    GridSensor.h
    Purpose: This is a generic interface used by all submodules
			 that will be reading in values from the grid

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef GridSensor_h
#define GridSensor_h

#include "Arduino.h"

#pragma once
class GridSensor
{
public:
  enum colour {WHITE, BLACK, ERR};
  colour sensorReading;

	GridSensor();
	~GridSensor();

  void test();
  colour getCurrentCell();
  bool hasChangedCell();
};

#endif
