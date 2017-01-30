/**
    GridSensor.cpp
    Purpose: This is a generic interface used by all submodules
			 that will be reading in values from the grid

       This class will offer feedback to the motors and so will need
       access to the movement class

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "GridSensor.h"

// See examples https://github.com/pololu/qtr-sensors-arduino/tree/master/examples
/*#include "QTRSensors.h"

#define NUM_SENSORS   2     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

unsigned int ERR   = 1;
unsigned int WHITE = 2;
unsigned int BLACK = 3;

// Create instance of sensors. Only using one sensor connected to pin 12
QTRSensorsAnalog qtra((unsigned char[]) {12},  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
unsigned int defaultValue    = 4000;
unsigned int colourThreshold = 2000;*/

// Using ADAFruit RGB colour sensors
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

GridSensor::GridSensor(sensorPosition sp)
{
  this->currentPosition = sp;

  // Setup the sensor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  
  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
      
    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;      
    }
    //Serial.println(gammatable[i]);
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
unsigned int GridSensor::getCurrentCell() {
  /*unsigned int reading;
  reading = qtra.readLine(sensorValues);
  
  if(reading == defaultValue){
    return ERR;
  }else if(reading <= colourThreshold){
    return WHITE;
  }else{
    return BLACK;
  }*/

  return 1;
}


bool GridSensor::hasChangedCell() {
  if(sensorReading == getCurrentCell()) {
    // Current cell has not changed
    return false;
  }else{
    return true;
  }
}


/*
 * This function will need to offer feedback to the motors
 */
void GridSensor::motorCorrection(){
  
}

void GridSensor::debugLoop(){
  uint16_t clear, red, green, blue;
  
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

  //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

  analogWrite(redpin, gammatable[(int)r]);
  analogWrite(greenpin, gammatable[(int)g]);
  analogWrite(bluepin, gammatable[(int)b]);
}


