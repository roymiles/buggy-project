/**
    UltraSonicSensor.h
    Purpose: This controls the logic required to interface with
			 the ultrasonic sensor hardware

    @author Roy Miles
    @version 1.0 11/12/2016

    HC-SR04 Ping distance sensor:
    VCC to arduino 5v 
    GND to arduino GND
    Echo to Arduino pin 7 
    Trig to Arduino pin 8
*/

#ifndef UltraSonicSensor_h
#define UltraSonicSensor_h

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

#include "Arduino.h"
#include "CollisionSensor.h" // This object is of type "CollisionSensor"

#pragma once
class UltraSonicSensor: public CollisionSensor
{
public:
	UltraSonicSensor();
	~UltraSonicSensor();
  
  int maximumRange = 200; // Maximum range needed
  int minimumRange = 0; // Minimum range needed
  long duration, distance; // Duration used to calculate distance

	// Return in cm the distance to an object
	float getDistanceToNearestObject();

  bool isCollision();
};

#endif
