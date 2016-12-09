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

	// Turn the buggy x degrees clockwise
	void turn(double degrees) { }

	// Turn the buggy 90 degrees anti-clockwise
	void turnLeft();

	// Turn the buggy 90 degrees clockwise
	void turnRight();

	// Move forward a distance of x
	void moveForward(double distance) { }

	// Move forward one square
	void moveForwardOne() { }

	// Cease all movement
	void stop() { }

	// Perform the victory roll
	void victoryRoll() { }
};

#endif

