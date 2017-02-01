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
  /*
   * First colour readings
   */
  colour initialSensorReading;
  
  sensorPosition currentPosition;
  const uint16_t grayScaleThreshold = 243; // Lower: black, Higher: white

	GridSensor();
	~GridSensor();

  void test();

  /**
   * Retrieve the current cell read by the IR colour sensors
   */
  colour getCurrentCell();

  /**
   * Returns true or false, depending on whether the current colour reading differs from the
   * initialSensorReading (which is set at the start of the motor movement by sensor
   * control)
   */
  bool hasChangedCell();

  /**
   * Convert RGB to BLACK/WHITE
   */
  colour convertToColour(uint16_t clear);

  /**
   * Print the response of the RGB colour sensors
   */
  void debug();
};

#endif

