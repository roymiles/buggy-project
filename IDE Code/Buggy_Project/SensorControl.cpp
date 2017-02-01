/**
    SensorControl.cpp
    Purpose: This is class encapsulates all the grid sensor and controls 
    the motors based on their response

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "SensorControl.h"

int SENSOR_MUX = 9;     // Output pin to control the MUX

SensorControl::SensorControl(Movement *m)
{
  this->m = m;

  // Mux output
  pinMode(SENSOR_MUX, OUTPUT);
  
  digitalWrite(SENSOR_MUX, LOW);
  delay(100);
  
  // Create instances of the grid sensor
  gs = new GridSensor();
}

SensorControl::~SensorControl()
{
}

void SensorControl::debug(){
  enableLeftSensor();
  gs->debug();

  enableRightSensor();
  gs->debug();
}

colour SensorControl::debugColour(){
  return gs->getCurrentCell();
}


void SensorControl::enableLeftSensor(){
  Serial.println("Enabling left colour sensor:");
  digitalWrite(SENSOR_MUX, HIGH);
  gs->initialSensorReading = initialSensorReading_topLeft;
}

void SensorControl::enableRightSensor(){
  Serial.println("Enabling right colour sensor:");
  digitalWrite(SENSOR_MUX, LOW);
  gs->initialSensorReading = initialSensorReading_topRight; 
}

/**
 * Get the initial readings prior to movement
 */
void SensorControl::movementInit(){
  enableLeftSensor();
  initialSensorReading_topLeft  = gs->getCurrentCell();

  enableRightSensor();
  initialSensorReading_topRight = gs->getCurrentCell();

  /*
   * Check the starting position is valid
   */
  if(initialSensorReading_topLeft != initialSensorReading_topRight){
    Serial.println("Initial sensor readings are not the same!");
    Serial.println("Halting...");
    while (true) {
      ; // Halt execution
    }
  }
}

/**
 * Read from the IR RGB colour sensors and adjust the motor controls to compensate
 */
bool left_changed, right_changed;
void SensorControl::motorCorrection(){
  if(Movement::currentMovement == FORWARD || Movement::currentMovement == BACKWARDS){
    // The IR sensors should see the same colour for all movement

    enableLeftSensor();
    left_changed  = gs->hasChangedCell();

    enableRightSensor();
    right_changed = gs->hasChangedCell(); 

    // Left sensor has changed but right sensor has not
    if(left_changed && !right_changed){
      // Need to turn slightly to the RIGHT to compensate

      // Increase LEFT motor by 1 and reduce RIGHT motor by 1
      this->m->increaseLeftMotor();
      this->m->decreaseRightMotor();

    // Right sensor has changed but left sensor has not      
    }else if(right_changed && !left_changed){
      // Need to turn slightly to the LEFT to compensate

      // Increase RIGHT motor by 1 and reduce LEFT motor by 1
      this->m->increaseRightMotor();
      this->m->decreaseLeftMotor();

    // Both sensors have changed value (since initial reading) and so finish movement
    }else if(right_changed && left_changed){
      this->m->stopMovement();
    }
  }else if(Movement::currentMovement == TURNING_LEFT){
    /* 
     * Stop movement when LEFT sensor has changed cell
     */
    enableLeftSensor();
    left_changed  = gs->hasChangedCell();

    enableRightSensor();
    right_changed = gs->hasChangedCell(); 
     
    if(left_changed && right_changed){
      this->m->stopMovement();
    }
    
    
  }else if(Movement::currentMovement == TURNING_RIGHT){
    /* 
     * Stop movement when RIGHT sensor has changed cell
     */
    enableLeftSensor();
    left_changed  = gs->hasChangedCell();

    enableRightSensor();
    right_changed = gs->hasChangedCell(); 
     
    if(left_changed && right_changed){
      this->m->stopMovement();
    }
  }
}


/*
 * Convert the movement enum to a string (for debugging)
 */
String SensorControl::getColour(colour c){
  switch(c){
    case WHITE:
      return "WHITE";
      break;
    case BLACK:
      return "BLACK";
      break;
    default:
      return "UNKNOWN";
  }
}


