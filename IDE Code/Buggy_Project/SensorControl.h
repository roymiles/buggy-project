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
#include "Communication.h" // So the module can reset/change the current slave state

#pragma once

#define THRESHOLD_MAX 1800
#define THRESHOLD_MIN 1000

// Input pins to the reed switches
const int REED_SWITCH1 = A2; 
const int REED_SWITCH2 = A1; 

enum colour {UNKNOWN_COLOUR, WHITE, BLACK};
// enum sensorPosition {UNKNOWN_POSITION, TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};
enum colourState {UNKNOWN_COLOUR_STATE, WHITE_BLACK, BLACK_WHITE};
enum crossOrLine {CROSS, LINE};

class SensorControl
{
public:
  /*
   * Public variables
   */
  unsigned int positionState; // See Notes

  unsigned int sideSensorThreshold[SIDE_NUM_SENSORS] = {1000, 1000};
  colour sideSensorColours[SIDE_NUM_SENSORS];

  unsigned int frontSensorThreshold[FRONT_NUM_SENSORS] = {1800, 1800, 1800};
  colour frontSensorColours[FRONT_NUM_SENSORS];  
  
  static colourState currentColourState; // Will alternate at start of movement init
  static crossOrLine currentPositionState;

  /* 
   * If the buggy finished previous movement due to a collision and runs into a collision on current movement 
   * then don't increment the coordinates
   */
  unsigned int consecutiveCollisions = 0; 

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


public:
  /*
   * Public functions
   */
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

  /**
   * Calibrate the QTR sensors
   */
  void calibrate();

  /**
   * Limit the threshold value between THRESHOLD_MIN and THRESHOLD_MAX
   */
  unsigned int limitThreshold(unsigned int threshold);

  /**
   * Print the maximum and minimum values of the QTR sensors
   */
   void postCalibration();

  /**
   * Wiggle the buggy left and right until the front sensors have opposite polarity
   */
  bool wiggleBuggy(movements pm);

  /**
   * Adjust the buggys orientation until the two reed switches return 1 (docked)
   */
  bool adjustDockingPosition();

  /**
   * Control the MUX to enable/disable communication with appropriate IR Grid sensor
   */
  void enableLeftSensor();
  void enableRightSensor();

  /**
   * Read from the IR RGB colour sensors and adjust the motor controls to compensate
   */
  void motorCorrection();
  void checkTransitions();
  void resetFlagsAndCounts();

  /**
   * Following functions are for the side/front sensors
   * Get the colours measured form the side/front sensors
   */
  void getSideSensorColours();
  void getFrontSensorColours();
  colour convertSideSensorValueToColour(unsigned int value, unsigned int index);
  colour convertFrontSensorValueToColour(unsigned int value, unsigned int index);

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

