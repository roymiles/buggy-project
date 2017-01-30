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

enum colour {UNKNOWN_COLOUR, WHITE, BLACK};
enum sensorPosition {UNKNOWN_POSITION, TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};

#pragma once
class GridSensor
{
public:
  colour firstSensorReading; // First colour reading
  sensorPosition currentPosition;

	GridSensor(sensorPosition sp);
	~GridSensor();

  void test();

  /*
   * Retrieve the current cell read by the IR colour sensors
   */
  colour getCurrentCell();
  bool hasChangedCell();

  /**
   * Print the response of the RGB colour sensors
   */
  void debug();
};

#endif

