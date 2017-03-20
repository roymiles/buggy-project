/**
    Communication.h
    Purpose: This is a generic interface used by all submodules
			 that will be communicating with other components

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef Communication_h
#define Communication_h

#include "Arduino.h" // Used for Serial printing
#include <Wire.h>

#pragma once

#define SLAVE_ID 44
// Outside class scope so the type is easily accessible
enum I2C_COMMAND : int // Used for static conversions
{
  MASTER_IDLE = 0, 
  MASTER_MOVING = 1,
  MASTER_DOCKED = 2, 
  MASTER_ADJUSTING = 3, 
  MASTER_SEND_POSITION = 4,
  
  SLAVE_IDLE = 5,
  SLAVE_BUSY = 6,
  SLAVE_DOCKED = 7,
  
  I2C_NORTH = 8,
  I2C_EAST = 9,
  I2C_SOUTH = 10,
  I2C_WEST = 11,
  
  DEFAULT_SLAVE_STATE = 12,
  AWAITING_X = 13,
  AWAITING_Y = 14,
  AWAITING_ORIENTATION = 15,
  
  I2C_FORWARD = 16,
  I2C_BACKWARDS = 17,
  I2C_TURN_LEFT = 18,
  I2C_TURN_RIGHT = 19,

  I2C_MIN = 0,
  I2C_MAX = 19

  /*
   * Anything higher is interpreted as a data value
   */
};

class Communication
{
public:
  Communication();
  ~Communication();
  
  int dataToCommand(int d);
  int commandToData(I2C_COMMAND cmd);
  
  void sendPosition(int x, int y, I2C_COMMAND orientation);
  I2C_COMMAND getNextMovement();

  String commandToString(I2C_COMMAND cmd);
};

#endif

