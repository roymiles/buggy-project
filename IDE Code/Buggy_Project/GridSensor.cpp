/**
    GridSensor.cpp
    Purpose: This is a generic interface used by all submodules
			 that will be reading in values from the grid

       This class will offer feedback to the motors and so will need
       access to the movement class

    @author Roy Miles
    @version 1.0 11/12/2016
*/

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

#include "GridSensor.h"

// Using ADAFruit RGB colour sensors
#include <Wire.h>
#include "Adafruit_TCS34725.h"

/*
 * Initialise with specific int time and gain values
 */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

GridSensor::GridSensor(sensorPosition sp)
{
  this->currentPosition = sp;

  // Setup the sensor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    Serial.println("Halting execution...");
    while (1); // halt!
  }
}


GridSensor::~GridSensor()
{
}

void GridSensor::test() {
  /*qtra.read(sensorValues);

  // Print the sensor values
  unsigned char i;
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
  }
  Serial.println(" ");
  delay(250);*/
}

/*
 * Retrieve the current cell read by the IR colour sensors
 */
colour GridSensor::getCurrentCell() {
  /*
   * Get the raw data from the sensor
   */
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  //colorTemp = tcs.calculateColorTemperature(r, g, b);
  //lux = tcs.calculateLux(r, g, b);

  return this->convertToColour(c);
}


bool GridSensor::hasChangedCell() {
  if(initialSensorReading == getCurrentCell()) {
    // Current cell has not changed
    return false;
  }else{
    return true;
  }
}

void GridSensor::debug(){
  uint16_t clear, red, green, blue;

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);  
  
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  Serial.print("\t");
  Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  Serial.println();
}

colour GridSensor::convertToColour(uint16_t clear){
  //uint16_t average = (red + green + blue) / 3;
  if(clear > grayScaleThreshold){
    return WHITE;
  }else{
    return BLACK;
  }
}


