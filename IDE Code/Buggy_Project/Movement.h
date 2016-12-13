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

#pragma once
class Movement
{
public:
	Movement();
	~Movement();

	bool isTurning;
	bool isMoving;

  // pin map interrupts
  static void startInterrupts();

	// Turn the buggy 90 degrees anti-clockwise
	void turnLeft();

	// Turn the buggy 90 degrees clockwise
	void turnRight();

	// Move forward a distance of x
	void moveForward(double distance);

	// Move forward one square
	void moveForwardOne() { }

	// Cease all movement
	void stop() { }

	// Perform the victory roll
	void victoryRoll() { }
 // ISR handlers for Rotary Encoders
  void ISRRightEncoder();
  void ISRLeftEncoder();
private:
  // Turn the buggy x degrees clockwise
  void turn(double degrees);
 
};

#endif

