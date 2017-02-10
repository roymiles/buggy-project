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

  /* 
   * Mux output RGB Sensors 
   */
  /*pinMode(SENSOR_MUX, OUTPUT);
  digitalWrite(SENSOR_MUX, LOW);
  delay(100);*/
  
  // Create instances of the grid sensor
  // gs = new GridSensor();

  // These sensors are alligned with the wheels of the buggy
  sgs = new SideGridSensor();

  // These sensors are used as alternatives to the RGB sensors (less complex)
  fgs = new FrontGridSensor();
}

SensorControl::~SensorControl()
{
}

unsigned int sideSensorThreshold = 1000;
colour sideSensorColours[SIDE_NUM_SENSORS];
void SensorControl::getSideSensorColours(){
  sgs->readSideSensors(); // Make sure the latest values are measured

  sideSensorColours[0] = convertSideSensorValueToColour(sgs->sideSensorValues[0]);
  sideSensorColours[1] = convertSideSensorValueToColour(sgs->sideSensorValues[1]);
}

unsigned int frontSensorThreshold = 1800;
colour frontSensorColours[FRONT_NUM_SENSORS];
void SensorControl::getFrontSensorColours(){
  fgs->readFrontSensors(); // Make sure the latest values are measured

  frontSensorColours[0] = convertFrontSensorValueToColour(fgs->frontSensorValues[0]);
  frontSensorColours[1] = convertFrontSensorValueToColour(fgs->frontSensorValues[1]);
}

/*
 * Used to convert the simple sensor values to a WHITE/BLACK colour
 */
colour SensorControl::convertSideSensorValueToColour(unsigned int value){
  if(value < sideSensorThreshold){
    return WHITE;
  }else{
    return BLACK;
  }
}

colour SensorControl::convertFrontSensorValueToColour(unsigned int value){
  if(value < frontSensorThreshold){
    return WHITE;
  }else{
    return BLACK;
  }
}

void SensorControl::debug(){
  /*
   * Debugging the front RGB sensors
   */
  /*enableLeftSensor();
  gs->debug();

  enableRightSensor();
  gs->debug();*/

  Serial.println("DEBUG>>>");
  Serial.println(">>> Front Sensors");
  getFrontSensorColours();
  Serial.print("FRONT LEFT: ");
  Serial.println(colourToString(frontSensorColours[0]));
  Serial.print("FRONT RIGHT: ");
  Serial.println(colourToString(frontSensorColours[1]));
  
  Serial.println(">>> Side Sensors");
  getSideSensorColours();
  Serial.print("SIDE LEFT: ");
  Serial.println(colourToString(sideSensorColours[0]));
  Serial.print("SIDE RIGHT: ");
  Serial.println(colourToString(sideSensorColours[1]));
  Serial.println("END DEBUG>>>");
}

colour SensorControl::debugColour(){
  //return gs->getCurrentCell();
  Serial.println("OBSELETE FUNCTION");
  return WHITE;
}


/*void SensorControl::enableLeftSensor(){
  Serial.println("xxxx Enabling left colour sensor:");
  digitalWrite(SENSOR_MUX, HIGH);
  // Persist the LEFT or RIGHT sensor value each time it is selected as there is only one grid sensor instance
  gs->initialSensorReading = initialSensorReading_left;
  Serial.println("xxxx Initial reading left: " + colourToString(initialSensorReading_left));
  delay(100);  
}

void SensorControl::enableRightSensor(){
  Serial.println("xxxx Enabling right colour sensor:");
  digitalWrite(SENSOR_MUX, LOW);
  gs->initialSensorReading = initialSensorReading_right; 
  Serial.println("xxxx Initial reading right: " + colourToString(initialSensorReading_left));
  delay(100);
}*/

/**
 * Get the initial readings prior to movement
 * @return true or false depending on whether initial starting position is correct
 */
bool frontSensorsStartedOnSameColour = false;
bool SensorControl::movementInit(movements pm){  
  Serial.println("movementInit");
  
  Serial.print("Previous movement: ");
  Serial.println(m->getMovement(pm));

  Serial.println("---- Getting FRONT sensor measurements...");
  /*
   * RGB Sensors
   */
  /*enableLeftSensor();
  initialSensorReading_left  = gs->getCurrentCell();

  enableRightSensor();
  initialSensorReading_right = gs->getCurrentCell();*/

  /*
   * Simple Front Sensors (alternative to the RGB Sensors)
   */
  getFrontSensorColours();  
  initialSensorReading_left  = frontSensorColours[0];
  initialSensorReading_right = frontSensorColours[1];

  Serial.print("---- Left front sensor: ");
  Serial.println(colourToString(initialSensorReading_left));

  Serial.print("---- Right front sensor: ");
  Serial.println(colourToString(initialSensorReading_right));   

  Serial.println("---- Getting SIDE sensor measurements...");
  getSideSensorColours();
  initialSideSensorReading_left  = sideSensorColours[0];
  initialSideSensorReading_right = sideSensorColours[1];

  /*
   * Side Sensors
   */
  Serial.print("---- Left side sensor: ");
  Serial.println(colourToString(initialSideSensorReading_left));

  Serial.print("---- Right side sensor: ");
  Serial.println(colourToString(initialSideSensorReading_right));  

  /*
   * Check the starting position is valid
   * Each sensor should read opposite colours
   */
  if(initialSensorReading_left == initialSensorReading_right){
    Serial.println("---- Initial sensor readings are the same!");
    colour startingSquare = initialSensorReading_left; // This is equal to the right reading
    switch(currentPositionState){
      case WHITE_BLACK:
        if(isValidColour(startingSquare)){ // If valid startingSquare
          /*
           * Set the initial values to what they are supposed to be
           * The motorCorrection will comepensate in real time
           */

          frontSensorsStartedOnSameColour = true;
          if(pm == BACKWARDS || pm == TURNING_LEFT || pm == TURNING_RIGHT){
             // Opposite polarity for backwards or turns
            Serial.println("Front sensors should be BLACK_WHITE");
            initialSensorReading_left  = BLACK;
            initialSensorReading_right = WHITE;
          }else{
            Serial.println("Front sensors should be WHITE_BLACK");
            initialSensorReading_left  = WHITE;
            initialSensorReading_right = BLACK;
          }
        }else{
          Serial.println("---- Unknown grid square");
          return false;
        }
        break;

      case BLACK_WHITE:
        if(isValidColour(startingSquare)){ // If valid startingSquare
          /*
           * Set the initial values to what they are supposed to be
           * The motorCorrection will comepensate in real time
           */

          if(pm == BACKWARDS){
             // Opposite polarity for backwards
             Serial.println("Front sensors should be WHITE_BLACK");
            initialSensorReading_left  = WHITE;
            initialSensorReading_right = BLACK;
          }else{
            Serial.println("Front sensors should be BLACK_WHITE");
            initialSensorReading_left  = BLACK;
            initialSensorReading_right = WHITE;
          }
          
        }else{
          Serial.println("---- Unknown grid square");
          return false;
        }
        break;

      default:
        Serial.println("---- Unknown position state.");
        return false;
    }
    
    //debug();
    return true;
  }else{
    frontSensorsStartedOnSameColour = false; // Opposite values
  }

  //debug();
  return true;
}

bool SensorControl::isValidColour(colour c){
  if(c == WHITE || c == BLACK){
    return true;
  }else{
    return false;
  }
}

/**
 * Get the current position of the buggy WHITE_BLACK | BLACK_WHITE
 */
void SensorControl::getStartPosition(){
  getSideSensorColours();
  initialSideSensorReading_left = convertSideSensorValueToColour(sgs->sideSensorValues[0]);
  initialSideSensorReading_right = convertSideSensorValueToColour(sgs->sideSensorValues[1]);

  if(initialSideSensorReading_left == WHITE && initialSideSensorReading_right == BLACK){
      Serial.println("Start position WHITE_BLACK");
      currentPositionState = WHITE_BLACK; 
  }else if(initialSideSensorReading_left == BLACK && initialSideSensorReading_right == WHITE){
      Serial.println("Start position BLACK_WHITE");
      currentPositionState = BLACK_WHITE; 
  }else{
    Serial.println("Side sensors have the same value... Halting...");
    while(true){
      ; // Halting
    }
  }
}

/**
 * Read from the IR RGB colour sensors and adjust the motor controls to compensate
 * Read from the side sensor to determine if movement has finished
 */
bool left_changed, right_changed, sideLeft_changed, sideRight_changed;
unsigned int motorCorrectionDelay = 10;
bool frontSensorsFlipped = false; 

enum deviatingState {NONE, LEFT_NOT_CHANGED, RIGHT_NOT_CHANGED, CHANGED};
static deviatingState currentDeviatingState = NONE;

bool isDeviatingAndHasNotChanged = false;
unsigned int turnCounter = 0; // Used for determining when a rotation has finished
void SensorControl::motorCorrection(){
  /*
  * RGB Sensors
  */
  /*enableLeftSensor();
  left_changed  = gs->hasChangedCell();
  
  enableRightSensor();
  right_changed = gs->hasChangedCell(); */
  
  /*
   * Simple Sensors
   */
   
  getFrontSensorColours();
  left_changed  = (initialSensorReading_left  != frontSensorColours[0]);
  right_changed = (initialSensorReading_right != frontSensorColours[1]);  
  
  switch(Movement::currentMovement){

    // Forward and backwards use the same logic
    case FORWARD:
    case BACKWARDS:

    
      // Front sensors moved onto next square, so flip the polarity
      if(frontSensorsFlipped == false && left_changed && right_changed){
        Serial.println("Front sensors flipped");
        frontSensorsFlipped = true;
        if(initialSensorReading_left == WHITE){
          initialSensorReading_left = BLACK;
        }else{
          initialSensorReading_left = WHITE;
        }
    
        if(initialSensorReading_right == WHITE){
          initialSensorReading_right = BLACK;
        }else{
          initialSensorReading_right = WHITE;
        }    
      }

      /*
       * Check if buggy has moved onto the next cell 
       */
      getSideSensorColours();
      sideLeft_changed  = (initialSideSensorReading_left  != sideSensorColours[0]);
      sideRight_changed = (initialSideSensorReading_right != sideSensorColours[1]);

    
      // The IR sensors should see the same colour for all movement

      /*
       * Determine whether the sensors are now reading different values
       */  
  
      // Left sensor has changed but right sensor has not
      if(right_changed && !left_changed && currentDeviatingState != RIGHT_NOT_CHANGED){
        // Need to turn slightly to the RIGHT to compensate

        if(currentDeviatingState = NONE){
          currentDeviatingState = LEFT_NOT_CHANGED;
        }
        
        // They are not equal
        if(initialSensorReading_left != initialSensorReading_right){
          currentDeviatingState = CHANGED;
        }
  
        Serial.println("Deviating to the LEFT. Attempting to compensate... Right changed and left has not");
        if(Movement::currentMovement == FORWARD){
          
          m->disableRightMotor();
          m->enableLeftMotor();
          
        }else if(Movement::currentMovement == BACKWARDS){
          /*
           * Opposite polarity for going backwards
           */
          m->disableLeftMotor();
          m->enableRightMotor();
          //m->disableRightMotor();
          //m->enableLeftMotor();
           
        }

        // Give a bit of time for the correction to take affect on the buggy position
        //delay(motorCorrectionDelay);
        
      // Right sensor has changed but left sensor has not      
      }else if(left_changed && !right_changed && currentDeviatingState != LEFT_NOT_CHANGED){
        // Need to turn slightly to the LEFT to compensate

        if(currentDeviatingState = NONE){
          currentDeviatingState = RIGHT_NOT_CHANGED;
        }

        // They are not equal
        if(initialSensorReading_left != initialSensorReading_right){
          currentDeviatingState = CHANGED;
        }
  
        Serial.println("Deviating to the RIGHT. Attempting to compensate... Left changed and right has not");
        if(Movement::currentMovement == FORWARD){
          
          m->disableLeftMotor();
          m->enableRightMotor();
  
        }else if(Movement::currentMovement == BACKWARDS){
          /*
           * Opposite polarity for going backwards
           */
          //m->disableLeftMotor();
          //m->enableRightMotor();
          m->disableRightMotor();
          m->enableLeftMotor();
          
        }
        //delay(motorCorrectionDelay);
  
      }else{
        // On track, so keep both motors enabled
        currentDeviatingState = NONE;
        m->enableLeftMotor();
        m->enableRightMotor();
        //delay(motorCorrectionDelay);
      }

      /*
       * If both the wheel sensors have changed
       */
      if(sideLeft_changed && sideRight_changed){
        togglePositionState();
        frontSensorsFlipped = false; // Reset
        Movement::stopMovement();
      }
      
      break;

    case TURNING_LEFT:
      /* 
       * Stop movement when LEFT sensor has changed cell
       */

      getSideSensorColours();
      sideLeft_changed  = (initialSideSensorReading_left  != sideSensorColours[0]);
      sideRight_changed = (initialSideSensorReading_right != sideSensorColours[1]);             
       
      if(left_changed){
        if(sideSensorColours[0] != sideSensorColours[1]){ // Check side sensors
          togglePositionState();
          Movement::stopMovement();
        }
      }
      
      break;
      
    case TURNING_RIGHT:
      /* 
       * Stop movement when RIGHT sensor has changed cell
       */

      getSideSensorColours();
      sideLeft_changed  = (initialSideSensorReading_left  != sideSensorColours[0]);
      sideRight_changed = (initialSideSensorReading_right != sideSensorColours[1]);       
       
      if(right_changed){ // Check front sensors
        if(sideSensorColours[0] != sideSensorColours[1]){ // Check side sensors
          togglePositionState();
          Movement::stopMovement();
        }
      }
      
      break;
    
    default:
      Serial.println("Code should not reach here... Halting...");
      while(true){
        ; // Suspend the program
      }
      
  } // end switch 
  
} // end func

void SensorControl::togglePositionState(){
  if(currentPositionState == WHITE_BLACK){
    Serial.println("BLACK_WHITE state");
    currentPositionState = BLACK_WHITE;
  }else{
    Serial.println("WHITE_BLACK state");
    currentPositionState = WHITE_BLACK;
  } 
}


/**
 * Convert the movement enum to a string (for debugging)
 */
String SensorControl::colourToString(colour c){
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


