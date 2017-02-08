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
#include "SideGridSensor.h"

#pragma once

class SensorControl
{
public:
  GridSensor *gs;
  SideGridSensor *sgs;
  colour initialSensorReading_left;
  colour initialSensorReading_right;
  
  enum positionState {NA, WHITE_BLACK, BLACK_WHITE};
  positionState currentPositionState = BLACK_WHITE; // Will alternate at start of movement init!

  SensorControl(Movement *m);
  ~SensorControl();

  /**
   * Get the initial readings prior to movement
   * @return true or false depending on whether successful
   */
  bool movementInit();

  void debug();
  colour debugColour();

  /**
   * Control the MUX to enable/disable communication with appropriate IR Grid sensor
   */
  void enableLeftSensor();
  void enableRightSensor();

  /**
   * Read from the IR RGB colour sensors and adjust the motor controls to compensate
   */
  void motorCorrection();

  /**
   * Following functions are for the side sensors
   * Get the colours measured form the side sensors
   */
  void getSideSensorColours();
  colour SensorControl::convertSideSensorValueToColour(unsigned int value);

  /**
   * Convert enums to strings (typically for debugging at serial output)
   * @return string output
   */
  String getColour(colour c);
  String getPositionState(positionState ps);
private:
  Movement *m;

};

#endif

