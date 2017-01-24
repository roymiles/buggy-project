/**
    GridSensor.cpp
    Purpose: This is a generic interface used by all submodules
			 that will be reading in values from the grid

    @author Roy Miles
    @version 1.0 11/12/2016
*/

// See examples https://github.com/pololu/qtr-sensors-arduino/tree/master/examples
#include "QTRSensors.h"

#include "GridSensor.h"

#define NUM_SENSORS   2     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

// Create instance of sensors. Only using one sensor connected to pin 12
QTRSensorsAnalog qtra((unsigned char[]) {12},  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
unsigned int defaultValue    = 4000;
unsigned int colourThreshold = 2000;

GridSensor::GridSensor()
{
}


GridSensor::~GridSensor()
{
}

void GridSensor::test() {
  qtra.read(sensorValues);

  // Print the sensor values
  unsigned char i;
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
  }
  Serial.println(" ");
  delay(250);
}

colour GridSensor::getCurrentCell() {
  unsigned int reading;
  reading = qtra.read(sensorValues);
  
  if(reading == defaultValue){
    return ERR;
  }else if(reading <= colourThreshold){
    return WHITE;
  }else{
    return BLACK;
  }
}

bool GridSensor::hasChangedCell() {
  if(sensorReading == this->getCurrentCell()) {
    // Current cell has not changed
    return false;
  }else{
    return true;
  }
}

