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

unsigned int FrontGridSensor::getMinimum(unsigned int index){
  return qtrrc_frontSensors.calibratedMinimumOn[index];
}

unsigned int FrontGridSensor::getMaximum(unsigned int index){
  return qtrrc_frontSensors.calibratedMaximumOn[index];
}

FrontGridSensor::FrontGridSensor(){
  Serial.println("Front grid sensor instance");

//  int i;
//  for (i = 0; i < FRONT_NUM_SENSORS; i++)
//  {
//    qtrrc_frontSensors.calibrate();
//    delay(20);
//  }
//  
//  for (i = 0; i < FRONT_NUM_SENSORS; i++)
//  {
//    Serial.print(qtrrc_frontSensors.calibratedMinimumOn[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//  // print the calibration maximum values measured when emitters were on
//  for (i = 0; i < FRONT_NUM_SENSORS; i++)
//  {
//    Serial.print(qtrrc_frontSensors.calibratedMaximumOn[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//  Serial.println();
//  delay(1000);  
//
//  // Line detection
//  unsigned int position = qtrrc_frontSensors.readLine(frontSensorValues);
//  //qtrrc.read(sensorValues);
//  // print the sensor values as numbers from 0 to 9, where 0 means maximum reflectance and
//  // 9 means minimum reflectance, followed by the line position
//  unsigned char j;
//  for (j = 0; j < FRONT_NUM_SENSORS; j++)
//  {
//    Serial.print(frontSensorValues[j] * 10 / 1001);
//    Serial.print(' ');
//  }
//  Serial.print("    ");
//  Serial.println(position);
//  delay(250);  
//  // End line detection
//
//  Serial.println(F("--------------------------"));
//  Serial.println(F("END Front grid sensor setup"));
//  Serial.println(F("--------------------------"));    
}

FrontGridSensor::~FrontGridSensor(){
  
}

void FrontGridSensor::calibrate(){
  qtrrc_frontSensors.calibrate();  
  delay(20);
}

void FrontGridSensor::postCalibration(){
  Serial.print("Front Left minimum: ");
  Serial.println(qtrrc_frontSensors.calibratedMinimumOn[0]);
  Serial.print("Front Right minimum: ");
  Serial.println(qtrrc_frontSensors.calibratedMinimumOn[1]);
  Serial.print("Front Right Backup minimum: ");
  Serial.println(qtrrc_frontSensors.calibratedMinimumOn[2]);

  Serial.print("Front Left maximum: ");
  Serial.println(qtrrc_frontSensors.calibratedMaximumOn[0]);
  Serial.print("Front Right maximum: ");
  Serial.println(qtrrc_frontSensors.calibratedMaximumOn[1]);  
  Serial.print("Front Right Backup maximum: ");
  Serial.println(qtrrc_frontSensors.calibratedMaximumOn[2]);  
}

/**
 * Values = 2500 are BLACK low values are WHITE
 * Generally, large values are BLACK and lower values are WHITE (this is contrary to the RGB sensors)
 */
void FrontGridSensor::readFrontSensors(){
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
