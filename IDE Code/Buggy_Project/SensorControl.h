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
#include "Movement.h"

#pragma once
class SensorControl
{
public:
  GridSensor *gs_top_left; // Top left
  GridSensor *gs_top_right; // Top right

  SensorControl(Movement *m);
  ~SensorControl();

  /**
   * Get the initial readings prior to movement
   */
  void movementInit();

  void debug();
  colour debugColour();

  /**
   * Read from the IR RGB colour sensors and adjust the motor controls to compensate
   */
  void motorCorrection();

  /**
   * Convert enums to strings (typically for debugging at serial output)
   */
  String getColour(colour c);
private:
  Movement *m;

};

#endif

