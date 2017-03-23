/**
    Movement.h
    Purpose: This is a submodule that will be used to
       to interact with the motors of the buggy

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef Movement_h
#define Movement_h

#include "Arduino.h"
#include "TimerOne.h" // Used for timer interrupts
#include "Communication.h" // So the module can reset/change the current slave state

#pragma once

enum movements {IDLE, FORWARD, BACKWARDS, TURNING_LEFT, TURNING_RIGHT, DOCKING};
enum movementCompensation {ON_TRACK, COMPENSATING_LEFT, COMPENSATING_RIGHT};
static unsigned int timerCounter = 0;

class Movement
{
public:
  /*
   * Public variables
   */

  static bool isCalibrating; // Gets set to false when a button is pressed

  // Max of 255
  static unsigned int defaultRotationalSpeed;
  static unsigned int defaultMovementSpeed;
  static unsigned int defaultSkidSpeed;   
   
  static bool isWiggling;
  static movements currentMovement;
  static movementCompensation currentMovementCompensation;

public:
  /*
   * Public functions
   */
   
  Movement();
  ~Movement();
  
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

  void enableLeftMotor();
  void disableLeftMotor();
  void enableRightMotor();
  void disableRightMotor();
  
  // ISR handler for timer
  static void timerIsr();

  // Convert enums to strings (typically for debugging at serial output)
  String getMovement(movements cm);

  static String movementStateToString(movementCompensation ms);
 
};

#endif


