#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"

#pragma once
class Sensors
{
public:
	Sensors();
	~Sensors();

	// Return in mm the distance to an object from the front sensor
	double readFrontSensor() { }
};

#endif