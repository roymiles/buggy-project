/**
    SensorControl.h
    Purpose: This is class encapsulates all the grid sensor

    @author Roy Miles
    @version 1.0 11/12/2016
*/


#ifndef SensorControl_h
#define SensorControl_h

#include "Arduino.h"
//#include "GridSensor.h"
#include "Movement.h"
#include "SideGridSensor.h"
#include "FrontGridSensor.h"

#pragma once

enum colour {UNKNOWN_COLOUR, WHITE, BLACK};
enum sensorPosition {UNKNOWN_POSITION, TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};

enum colourState {NA, WHITE_BLACK, BLACK_WHITE};
enum crossOrLine {CROSS, LINE};

class SensorControl
{
public:
  unsigned int positionState; // See Notes
  
  static colourState currentColourState; // Will alternate at start of movement init
  static crossOrLine currentPositionState;

  //GridSensor *gs;
  colour initialSensorReading_left;
  colour initialSensorReading_leftBackup; // Chip not connected yet
  colour initialSensorReading_right;
  colour initialSensorReading_rightBackup;

  SideGridSensor *sgs;
  colour initialSideSensorReading_left;
  colour initialSideSensorReading_right;

  FrontGridSensor *fgs;
  // Uses the same values as GridSensor
  /*colour initialFrontSensorReading_left;
  colour initialFrontSensorReading_right;*/

  SensorControl(Movement *m);
  ~SensorControl();

  void getStartPosition();
  void getPositionState();
  void toggleColourState();
  bool isValidColour(colour c);
  unsigned int colourToNumber(colour c);

  /**
   * Get the initial readings prior to movement and adjust the buggy if required
   */
  void movementInit(movements pm, movements cm);

  void debug();
  colour debugColour();

  /**
   * Wiggle the buggy left and right until the front sensors have opposite polarity
   */
  bool wiggleBuggy();

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
   * Following functions are for the side/front sensors
   * Get the colours measured form the side/front sensors
   */
  void getSideSensorColours();
  void getFrontSensorColours();
  colour convertSideSensorValueToColour(unsigned int value);
  colour convertFrontSensorValueToColour(unsigned int value);

  /**
   * Convert enums to strings (typically for debugging at serial output)
   * @return string output
   */
  String colourToString(colour c);
  String getColourState(colourState cs);
private:
  Movement *m;

};

#endif

