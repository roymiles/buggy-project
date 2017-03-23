/**
    Communication.h
    Purpose: This class is used to communicate to the SLAVE device in
    order to transmit and recieve data from the basestation

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef Communication_h
#define Communication_h

#include "Arduino.h" // Used for Serial printing
#include "Point.h"
#include <Wire.h>

#pragma once

#define SLAVE_ID 44

// Outside class scope so the type is easily accessible
enum I2C_COMMAND : int // Used for static conversions
{
  /*
   * All the different states the buggy can be in
   */
  SLAVE_IDLE = 0,
  SLAVE_MOVING = 1,
  SLAVE_WIGGLING = 2,
  SLAVE_FINISHED = 3,
  SLAVE_STOPPED = 4,
  SLAVE_DOCKED = 5,
  SLAVE_DOCKING = 6,
  
  I2C_NORTH = 7,
  I2C_EAST = 8,
  I2C_SOUTH = 9,
  I2C_WEST = 10,
  
  I2C_FORWARD = 11,
  I2C_BACKWARDS = 12,
  I2C_TURN_LEFT = 13,
  I2C_TURN_RIGHT = 14,
  I2C_VICTORY = 15, // Removed
  I2C_DO_NOTHING = 16,
  I2C_AT_TARGET = 17,

  I2C_MIN = 0,
  I2C_MAX = 17

  /*
   * Anything higher is interpreted as a data value
   */
};

enum orientation : int 
{
  NORTH = I2C_NORTH, 
  EAST = I2C_EAST, 
  SOUTH = I2C_SOUTH, 
  WEST = I2C_WEST
};

class Communication
{

public:
  /*
   * Public variables
   */

  /*
   * The following variables are manipulated by static (interrupt) functions
   * and so need to be static too
   */
  static Point *currentCoordinates;
  static int recievedVal;
  static I2C_COMMAND recievedCommand; // Recieved from the basestation
  static char dataBuffer[4];
  static orientation currentOrientation;
  static I2C_COMMAND curState;

public:
  /*
   * Public functions
   */
  Communication();
  ~Communication();
  
  static void setCurrentOrientation(orientation ornt);
  static void setCurState(I2C_COMMAND state);  
  
  int dataToCommand(int d);
  int commandToData(I2C_COMMAND cmd);
  
  // Master is requesting data from slave (buggy)
  static void requestEvent();
  // Master has sent data to the slave (buggy)
  static void receiveEvent(int bytes);

  static uint32_t convertDataToI2C(int x, int y, int orientation, I2C_COMMAND state);
  void convertI2CToData(int data, int *x, int *y, int *orientation, I2C_COMMAND *state);

  static String commandToString(I2C_COMMAND cmd);
};

#endif

