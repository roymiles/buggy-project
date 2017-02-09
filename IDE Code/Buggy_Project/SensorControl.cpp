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
  Serial.println("Sensor control instance");
  this->m = m;

  // Mux output
  pinMode(SENSOR_MUX, OUTPUT);
  
  digitalWrite(SENSOR_MUX, LOW);
  delay(100);
  
  // Create instances of the grid sensor
  gs = new GridSensor();

  // These sensors are alligned with the wheels of the buggy
  sgs = new SideGridSensor();
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
  digitalWrite(SENSOR_MUX, HIGH);
  gs->initialSensorReading = initialSensorReading_left;
  //Serial.println("Initial reading left: " + getColour(initialSensorReading_left));
  delay(100);  
}

void SensorControl::enableRightSensor(){
  //Serial.println("Enabling right colour sensor:");
  digitalWrite(SENSOR_MUX, LOW);
  gs->initialSensorReading = initialSensorReading_right; 
  //Serial.println("Initial reading right: " + getColour(initialSensorReading_left));
  delay(100);
}

/**
 * Get the initial readings prior to movement
 * @return true or false depending on whether initial starting position is correct
 */
bool SensorControl::movementInit(){  
  Serial.print("Start of movementInit()");
  enableLeftSensor();
  initialSensorReading_left  = gs->getCurrentCell();

  enableRightSensor();
  initialSensorReading_right = gs->getCurrentCell();

  this->getSideSensorColours();
  initialSideSensorReading_left = convertSideSensorValueToColour(sgs->sideSensorValues[0]);
  initialSideSensorReading_right = convertSideSensorValueToColour(sgs->sideSensorValues[1]);

  Serial.print("Left side sensor: ");
  Serial.println(getColour(initialSideSensorReading_left));

  Serial.print("Right side sensor: ");
  Serial.println(getColour(initialSideSensorReading_right));  

  // Alternate at the start of every movement
  if(currentPositionState == WHITE_BLACK){
    currentPositionState = BLACK_WHITE;
    Serial.println("Initial state is BLACK_WHITE");
    if(initialSensorReading_left == WHITE && initialSensorReading_right == BLACK){
      // On completely the wrong grid position
      Serial.println("Wrong position!");
    }
  }else{
    currentPositionState = WHITE_BLACK;
    Serial.println("Initial state is WHITE_BLACK");
    if(initialSensorReading_left == BLACK && initialSensorReading_right == WHITE){
      // On completely the wrong grid position
      Serial.println("Wrong position!");
    }
  }  

  Serial.println("LEFT sensor starting on:  " + getColour(initialSensorReading_left));
  Serial.println("RIGHT sensor starting on: " + getColour(initialSensorReading_right));

  /*
   * Check the starting position is valid
   * Each sensor should read opposite colours
   */
  if(initialSensorReading_left == initialSensorReading_right){
    Serial.println("Initial sensor readings are the same!");
    colour startingSquare = initialSensorReading_left;
    switch(currentPositionState){
      case WHITE_BLACK:
        if(startingSquare == WHITE || startingSquare == BLACK){
          /*
           * Set the initial values to what they are supposed to be
           * The motorCorrection will comepensate in real time
           */
          initialSensorReading_left  = WHITE;
          initialSensorReading_right = BLACK;
        }else{
          Serial.println("Unknown grid square");
          return false;
        }
        break;

      case BLACK_WHITE:
        if(startingSquare == WHITE || startingSquare == BLACK){
          /*
           * Set the initial values to what they are supposed to be
           * The motorCorrection will comepensate in real time
           */
          initialSensorReading_left  = BLACK;
          initialSensorReading_right = WHITE;
        }else{
          Serial.println("Unknown grid square");
          return false;
        }
        break;

      default:
        Serial.println("Unknown position state.");
        return false;
    }
    
    debug();
    return true;
  }

  debug();
  return true;
}

unsigned int sideSensorThreshold = 1000;
colour sideSensorColours[NUM_SENSORS];
void SensorControl::getSideSensorColours(){
  sgs->readSideSensors(); // Make sure the latest values are measured

  sideSensorColours[0] = this->convertSideSensorValueToColour(sgs->sideSensorValues[0]);
  sideSensorColours[1] = this->convertSideSensorValueToColour(sgs->sideSensorValues[1]);
}

colour SensorControl::convertSideSensorValueToColour(unsigned int value){
  if(value < sideSensorThreshold){
    return WHITE;
  }else{
    return BLACK;
  }
}

/**
 * Read from the IR RGB colour sensors and adjust the motor controls to compensate
 * Read from the side sensor to determine if movement has finished
 */
bool left_changed, right_changed, sideLeft_changed, sideRight_changed;
unsigned int motorCorrectionDelay = 100; 
unsigned int numberOfSideSensorChanges = 0; // Used for determining when a rotation has finished
void SensorControl::motorCorrection(){
  switch(Movement::currentMovement){
    case FORWARD:
    case BACKWARDS:
      // The IR sensors should see the same colour for all movement

      /*
       * Determine whether the sensors are now reading different values
       */
      enableLeftSensor();
      left_changed  = gs->hasChangedCell();
  
      enableRightSensor();
      right_changed = gs->hasChangedCell(); 
  
      // Left sensor has changed but right sensor has not
      if(right_changed && !left_changed){
        // Need to turn slightly to the RIGHT to compensate
  
        Serial.println("Deviating to the LEFT. Attempting to compensate...");
        if(Movement::currentMovement == FORWARD){
          
          this->m->disableRightMotor();
          this->m->enableLeftMotor();
          
        }else if(Movement::currentMovement == BACKWARDS){
          /*
           * Opposite polarity for going backwards
           */
          this->m->disableLeftMotor();
          this->m->enableRightMotor();
          
        }
        
        delay(motorCorrectionDelay);
        
      // Right sensor has changed but left sensor has not      
      }else if(left_changed && !right_changed){
        // Need to turn slightly to the LEFT to compensate
  
        Serial.println("Deviating to the RIGHT. Attempting to compensate...");
        if(Movement::currentMovement == FORWARD){
          
          this->m->disableLeftMotor();
          this->m->enableRightMotor();
  
        }else if(Movement::currentMovement == BACKWARDS){
          /*
           * Opposite polarity for going backwards
           */
          this->m->disableRightMotor();
          this->m->enableLeftMotor();
          
        }
        delay(motorCorrectionDelay);
  
      }else{
        // On track, so keep both motors enabled
        this->m->enableLeftMotor();
        this->m->enableRightMotor();
        delay(motorCorrectionDelay);
      }
  
      /*
       * Check if buggy has moved onto the next cell 
       */
      /*getSideSensorColours();
      sideLeft_changed  = (initialSideSensorReading_left != convertSideSensorValueToColour(sgs->sideSensorValues[0]));
      sideRight_changed = (initialSideSensorReading_right != convertSideSensorValueToColour(sgs->sideSensorValues[1]));
      
      if(sideLeft_changed && sideRight_changed){
        Movement::stopMovement();
      }*/
      
      break;

    case TURNING_LEFT:
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
      
      break;
      
    case TURNING_RIGHT:
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
      
      break;
    
    default:
      Serial.println("Code should not reach here... Halting...");
      while(true){
        ; // Suspend the program
      }
      
  } // end switch 
  
} // end func


/**
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

String SensorControl::getPositionState(positionState ps){
  switch(ps){
    case WHITE_BLACK:
      return "WHITE_BLACK";
      break;
    case BLACK_WHITE:
      return "BLACK_WHITE";
      break;
    default:
      return "UNKNOWN"; 
  }
}


