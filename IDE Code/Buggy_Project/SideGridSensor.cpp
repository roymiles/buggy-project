#include "SideGridSensor.h"

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white 
// paper and sliding the sensor across it.  It prints the sensor values to the serial 
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).

// sensors 0 through 1 are connected to digital pins 3 through 4, respectively

/*
 * LEFT sensor PIN 3
 * RIGHT sensor PIN 4
 */
QTRSensorsRC qtrrc_sideSensors((unsigned char[]) {9, 8}, SIDE_NUM_SENSORS, SIDE_TIMEOUT, SIDE_EMITTER_PIN); 

SideGridSensor::SideGridSensor(){
  Serial.println("Side grid sensor instance");

//  int i;
//  for (i = 0; i < SIDE_NUM_SENSORS; i++)
//  {
//    qtrrc_sideSensors.calibrate();
//    delay(20);
//  }
//
//  for (i = 0; i < SIDE_NUM_SENSORS; i++)
//  {
//    Serial.print(qtrrc_sideSensors.calibratedMinimumOn[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//  // print the calibration maximum values measured when emitters were on
//  for (i = 0; i < SIDE_NUM_SENSORS; i++)
//  {
//    Serial.print(qtrrc_sideSensors.calibratedMaximumOn[i]);
//    Serial.print(' ');
//  }
//  
//  Serial.println();
//  Serial.println();
//  delay(1000);  
//
//  // Line detection
//  unsigned int position = qtrrc_sideSensors.readLine(sideSensorValues);
//  //qtrrc.read(sensorValues);
//  // print the sensor values as numbers from 0 to 9, where 0 means maximum reflectance and
//  // 9 means minimum reflectance, followed by the line position
//  unsigned char j;
//  for (j = 0; j < SIDE_NUM_SENSORS; j++)
//  {
//    Serial.print(sideSensorValues[j] * 10 / 1001);
//    Serial.print(' ');
//  }
//  Serial.print("    ");
//  Serial.println(position);
//  delay(250);  
//  // End line detection  
//
//  Serial.println(F("--------------------------"));
//  Serial.println(F("END Side grid sensor setup"));
//  Serial.println(F("--------------------------"));  
}

unsigned int SideGridSensor::getMinimum(unsigned int index){
  return qtrrc_sideSensors.calibratedMinimumOn[index];
}

unsigned int SideGridSensor::getMaximum(unsigned int index){
  return qtrrc_sideSensors.calibratedMaximumOn[index];
}

SideGridSensor::~SideGridSensor(){
  
}

void SideGridSensor::calibrate(){
  qtrrc_sideSensors.calibrate();  
  delay(20);
}

void SideGridSensor::postCalibration(){
  Serial.print("Side Left minimum: ");
  Serial.println(qtrrc_sideSensors.calibratedMinimumOn[0]);
  Serial.print("Side Right minimum: ");
  Serial.println(qtrrc_sideSensors.calibratedMinimumOn[1]);

  Serial.print("Side Left maximum: ");
  Serial.println(qtrrc_sideSensors.calibratedMaximumOn[0]);
  Serial.print("Side Right maximum: ");
  Serial.println(qtrrc_sideSensors.calibratedMaximumOn[1]);  
}

/**
 * Values = 2500 are BLACK low values are WHITE
 * Generally, large values are BLACK and lower values are WHITE (this is contrary to the RGB sensors)
 */
void SideGridSensor::readSideSensors(){
  // read raw sensor values
  qtrrc_sideSensors.read(sideSensorValues);

  /* 
   * print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and 
   * 2500 means minimum reflectance
   */
  /*for (unsigned char i = 0; i < SIDE_NUM_SENSORS; i++)
  {
    if(i == 0){
      Serial.print("Left: ");
    }else{
      Serial.print("Right: ");
    }
    Serial.print(sideSensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }

  Serial.println();*/
}
