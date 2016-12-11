/**
    UltraSonicSensor.h
    Purpose: This controls the logic required to interface with
			 the ultrasonic sensor hardware

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef UltraSonicSensor_h
#define UltraSonicSensor_h

#include "Arduino.h"
#include "CollisionSensor.h" // This object is of type "CollisionSensor"

#pragma once
class UltraSonicSensor: public CollisionSensor
{
public:
	UltraSonicSensor();
	~UltraSonicSensor();

	// Return in mm the distance to an object
	double getDistanceToNearestObject();

  bool isCollision();
};

#endif
