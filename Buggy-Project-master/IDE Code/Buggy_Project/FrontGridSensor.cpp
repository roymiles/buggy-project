#include "FrontGridSensor.h"

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white 
// paper and sliding the sensor across it.  It prints the sensor values to the serial 
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).

// sensors 0 through 1 are connected to digital pins 2 through 3, respectively

/*
 * LEFT sensor PIN 12
 * RIGHT sensor PIN 10
 * RIGHT BACKUP sensor PIN 13
 */
QTRSensorsRC qtrrc_frontSensors((unsigned char[]) {12, 10, 13}, FRONT_NUM_SENSORS, FRONT_TIMEOUT, FRONT_EMITTER_PIN); 

FrontGridSensor::FrontGridSensor(){
  Serial.println("Front grid sensor instance");
}

FrontGridSensor::~FrontGridSensor(){
  
}

/**
 * Values = 2500 are BLACK low values are WHITE
 * Generally, large values are BLACK and lower values are WHITE (this is contrary to the RGB sensors)
 */
FrontGridSensor::readFrontSensors(){
  // read raw sensor values
  qtrrc_frontSensors.read(frontSensorValues);

  /* 
   * print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and 
   * 2500 means minimum reflectance
   */
  /*for (unsigned char i = 0; i < FRONT_NUM_SENSORS; i++)
  {
    if(i == 0){
      Serial.print("Left: ");
    }else{
      Serial.print("Right: ");
    }
    Serial.print(frontSensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }

  Serial.println();*/
}
