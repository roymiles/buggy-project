/**
    Movement.h
    Purpose: This is a submodule that will be used to
       to interact with the motors of the buggy
       (this doesn't need a generic interface as
       the buggys motors won't be changing)

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef Movement_h
#define Movement_h

#include "Arduino.h"
#include "TimerOne.h"

#pragma once

enum movements {FORWARD, BACKWARDS, TURNING_LEFT, TURNING_RIGHT, IDLE};
static movements currentMovement;
static unsigned int timerCounter = 0;

class Movement
{
public:

  Movement();
  ~Movement();
  
  void Test();
  // Turn the buggy 90 degrees anti-clockwise
  void turnLeft();

  // Turn the buggy 90 degrees clockwise
  void turnRight();

  // Move forward to the next cell
  void moveForward();

  // Move backwards to the previous cell
  void moveBackwards();

  // Cease all movement
  static void stopMovement();

  // Perform the victory roll
  void victoryRoll() { }
  
  // ISR handlers for Rotary Encoders
  static void ISRRightEncoder();
  static void ISRLeftEncoder();
  
  // ISR handler for timer
  static void timerIsr();
  
private:

  // Turn the buggy x degrees clockwise
  void turn(double degrees);
 
};

#endif

