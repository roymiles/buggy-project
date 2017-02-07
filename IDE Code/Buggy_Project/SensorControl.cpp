/**
    SensorControl.cpp
    Purpose: This is class encapsulates all the grid sensor and controls 
    the motors based on their response

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "SensorControl.h"

int SENSOR_MUX = 9;     // Output pin to control the MUX
enum positionState {NA, WHITE_BLACK, BLACK_WHITE};
positionState currentPositionState = BLACK_WHITE; // Will alternate at start of movement init!

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
  //Serial.println("Enabling left colour sensor:");
  digitalWrite(SENSOR_MUX, LOW);
  gs->initialSensorReading = initialSensorReading_topLeft;
  //Serial.println("Initial reading left: " + getColour(initialSensorReading_topLeft));
  delay(100);  
}

void SensorControl::enableRightSensor(){
  //Serial.println("Enabling right colour sensor:");
  digitalWrite(SENSOR_MUX, HIGH);
  gs->initialSensorReading = initialSensorReading_topRight; 
  //Serial.println("Initial reading right: " + getColour(initialSensorReading_topLeft));
  delay(100);
}

/**
 * Get the initial readings prior to movement
 */
bool SensorControl::movementInit(){  
  enableLeftSensor();
  initialSensorReading_topLeft  = gs->getCurrentCell();

  enableRightSensor();
  initialSensorReading_topRight = gs->getCurrentCell();

  // Alternate at the start of every init
  if(currentPositionState == WHITE_BLACK){
    currentPositionState = BLACK_WHITE;
    Serial.println("Initial state is BLACK_WHITE");
    if(initialSensorReading_topLeft == WHITE && initialSensorReading_topRight == BLACK){
      // On completely the wrong grid position
      Serial.println("Wrong position!");
    }
  }else{
    currentPositionState = WHITE_BLACK;
    Serial.println("Initial state is WHITE_BLACK");
    if(initialSensorReading_topLeft == BLACK && initialSensorReading_topRight == WHITE){
      // On completely the wrong grid position
      Serial.println("Wrong position!");
    }
  }  

  Serial.println("LEFT sensor starting on:  " + getColour(initialSensorReading_topLeft));
  Serial.println("RIGHT sensor starting on: " + getColour(initialSensorReading_topRight));

  delay(1000);

  /*
   * Check the starting position is valid
   * Each sensor should read opposite colours
   */
  if(initialSensorReading_topLeft == initialSensorReading_topRight){
    Serial.println("Initial sensor readings are the same!");
    colour startingSquare = initialSensorReading_topLeft;
    switch(currentPositionState){
      case WHITE_BLACK:
        Serial.println("WHITE_BLACK state");
        if(startingSquare == WHITE){
          // Started facing the left, so need to compensate
          // Set the values to what they should* be
          initialSensorReading_topLeft  = WHITE;
          initialSensorReading_topRight = BLACK;
        }else if(startingSquare == BLACK){
          // Started facing the right
          initialSensorReading_topLeft  = WHITE;
          initialSensorReading_topRight = BLACK;
        }else{
          Serial.println("Unknown grid square");
          return false;
        }
        break;

      case BLACK_WHITE:
        Serial.println("BLACK_WHITE state");
        if(startingSquare == WHITE){
          // Started facing the right, so need to compensate
          initialSensorReading_topLeft  = BLACK;
          initialSensorReading_topRight = WHITE;
        }else if(startingSquare == BLACK){
          // Started facing the left
          initialSensorReading_topLeft  = BLACK;
          initialSensorReading_topRight = WHITE;
        }else{
          Serial.println("Unknown grid square");
          return false;
        }
        break;

      default:
        Serial.println("Unknown position state.");
        return false;
    }
    //while (true) {
    //  ; // Halt execution
    //}
    debug();
    return true;
  }

  debug();
  return true;
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
    if(right_changed && !left_changed){
      // Need to turn slightly to the RIGHT to compensate

      Serial.println("Deviating to the LEFT. Attempting to compensate...");
      // Increase RIGHT motor by 1 and reduce LEFT motor by 1
      /*this->m->increaseLeftMotor();
      this->m->decreaseRightMotor();
      delay(50);*/
      this->m->disableRightMotor();
      this->m->enableLeftMotor();
      delay(50);
      
    // Right sensor has changed but left sensor has not      
    }else if(left_changed && !right_changed){
      // Need to turn slightly to the LEFT to compensate

      Serial.println("Deviating to the RIGHT. Attempting to compensate...");
      // Increase LEFT motor by 1 and reduce RIGHT motor by 1
      /*this->m->increaseLeftMotor();
      this->m->decreaseRightMotor();
      delay(50);*/
      this->m->disableLeftMotor();
      this->m->enableRightMotor();
      delay(50);

    // Both sensors have changed value (since initial reading) and so finish movement
    }else if(right_changed && left_changed){
      Movement::stopMovement();
    }else{
      // On track
      this->m->enableLeftMotor();
      this->m->enableRightMotor();
      delay(50);
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
      Movement::stopMovement();
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
      Movement::stopMovement();
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


