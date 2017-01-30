/**
    SensorControl.cpp
    Purpose: This is class encapsulates all the grid sensor and controls 
    the motors based on their response

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "SensorControl.h"


SensorControl::SensorControl(Movement *m)
{
  this->m = m;
  
  // Create instances of the grid sensors for each position
  gs_top_left   = new GridSensor(sensorPosition::TOP_LEFT);
  gs_top_right  = new GridSensor(sensorPosition::TOP_RIGHT);
}


SensorControl::~SensorControl()
{
}

/**
 * Get the initial readings prior to movement
 */
void SensorControl::movementInit(){
  gs_top_left->firstSensorReading  = gs_top_left->getCurrentCell();
  gs_top_right->firstSensorReading = gs_top_right->getCurrentCell();
}

/**
 * Read from the IR RGB colour sensors and adjust the motor controls to compensate
 */
bool left_changed, right_changed;
void SensorControl::motorCorrection(){
  if(Movement::currentMovement == FORWARD || Movement::currentMovement == BACKWARDS){
    // The IR sensors should see the same colour for all movement

    left_changed  = gs_top_left->hasChangedCell();
    right_changed = gs_top_right->hasChangedCell();

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
  }
}


