/**
    UltraSonicSensor.cpp
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


#include "UltraSonicSensor.h"

UltraSonicSensor::UltraSonicSensor()
{
  Serial.write("Ultrasonic sensor module...\n");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


UltraSonicSensor::~UltraSonicSensor()
{
}

double UltraSonicSensor::getDistanceToNearestObject(){
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  Serial.print("Duration: ");
  Serial.println(duration);
  
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  
  if (distance >= maximumRange || distance <= minimumRange){
    /* Send a negative number to computer and Turn LED ON 
    to indicate "out of range" */
    Serial.println("-1");
  }
  else {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    Serial.println(distance);
  }

  return distance;
}

bool UltraSonicSensor::isCollision(){
  // EMPTY
}

