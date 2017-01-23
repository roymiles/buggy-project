/**
    CollisionSensor.h
    Purpose: This is a generic interface used by all submodules
			 that will be detecting objects for collisions

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef CollisionSensor_h
#define CollisionSensor_h

#include "Arduino.h"

#pragma once
class CollisionSensor
{
public:
	CollisionSensor();
	~CollisionSensor();

	virtual bool isCollision() = 0;
};

#endif