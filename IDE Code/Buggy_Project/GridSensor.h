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
#include "Movement.h"

enum sensorPosition {NA, TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};

#pragma once
class GridSensor
{
public:
  unsigned int sensorReading;
  sensorPosition currentPosition;

	GridSensor(sensorPosition sp);
	~GridSensor();

  void test();
  void motorCorrection();

  /*
   * Retrieve the current cell read by the IR colour sensors
   */
  unsigned int getCurrentCell();
  bool hasChangedCell();
  void debugLoop();
};

#endif

