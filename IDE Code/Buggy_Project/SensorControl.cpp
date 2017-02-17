/**
    SensorControl.cpp
    Purpose: This is class encapsulates all the grid sensor and controls 
    the motors based on their response

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "SensorControl.h"

int SENSOR_MUX = 9;     // Output pin to control the MUX

colourState SensorControl::currentColourState = BLACK_WHITE; // Will alternate at start of movement init
crossOrLine SensorControl::currentPositionState = LINE;

SensorControl::SensorControl(Movement *m)
{
  Serial.println("Sensor control instance");
  this->m = m;

  // Initial values
  currentColourState = BLACK_WHITE; // Will alternate at start of movement init
  currentPositionState = LINE;  

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
  frontSensorColours[2] = convertFrontSensorValueToColour(fgs->frontSensorValues[2]);
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

  Serial.println("DEBUG >>>");
  Serial.println(">>> Front Sensors");
  Serial.print("Front threshold: ");
  Serial.println(frontSensorThreshold);
  
  getFrontSensorColours();
  Serial.print("FRONT LEFT: ");
  Serial.print(colourToString(frontSensorColours[0]));
  Serial.print(" ");
  Serial.println(fgs->frontSensorValues[0]);
  
  Serial.print("FRONT RIGHT: ");
  Serial.print(colourToString(frontSensorColours[1]));
  Serial.print(" ");
  Serial.println(fgs->frontSensorValues[1]);
  
  Serial.print("FRONT BACKUP: ");
  Serial.print(colourToString(frontSensorColours[2]));
  Serial.print(" ");
  Serial.println(fgs->frontSensorValues[2]);
  
  Serial.println(">>> Side Sensors");
  Serial.print("Side threshold: ");
  Serial.println(sideSensorThreshold);
  
  getSideSensorColours();
  Serial.print("SIDE LEFT: ");
  Serial.print(colourToString(sideSensorColours[0]));
  Serial.print(" ");
  Serial.println(sgs->sideSensorValues[0]);
  
  Serial.print("SIDE RIGHT: ");
  Serial.print(colourToString(sideSensorColours[1]));
  Serial.print(" ");
  Serial.println(sgs->sideSensorValues[1]);
  
  Serial.println("END DEBUG >>>");
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
 * Get the initial readings prior to movement and calculate the expected grid sqaures after movement
 */
bool frontSensorsStartedOnSameColour = false;
void SensorControl::movementInit(movements pm, movements cm){  
  Serial.print("Previous movement: ");
  Serial.println(m->getMovement(pm));
  Serial.print("Current movement: ");
  Serial.println(m->getMovement(cm));

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
  initialSensorReading_left         = frontSensorColours[0];
  initialSensorReading_right        = frontSensorColours[1];
  initialSensorReading_rightBackup  = frontSensorColours[2];
  Serial.print("---- Left front sensor: ");
  Serial.println(colourToString(initialSensorReading_left));
  Serial.print("---- Right front sensor: ");
  Serial.println(colourToString(initialSensorReading_right)); 
  Serial.print("---- Right backup front sensor: ");
  Serial.println(colourToString(initialSensorReading_rightBackup));   

  /*
   * Side Sensors
   */
  getSideSensorColours();
  initialSideSensorReading_left  = sideSensorColours[0];
  initialSideSensorReading_right = sideSensorColours[1];
  Serial.print("---- Left side sensor: ");
  Serial.println(colourToString(initialSideSensorReading_left));
  Serial.print("---- Right side sensor: ");
  Serial.println(colourToString(initialSideSensorReading_right));  

  // If the front sensors are initially different, wiggle the buggy
  if(initialSensorReading_left == initialSensorReading_right /*&& (cm != TURNING_RIGHT || cm != TURNING_LEFT)*/ ){
    wiggleBuggy(); // Wiggle the buggy until the front sensors are different
  }

  // Get the buggy position state CROSS or LINE and the start grid colours
  getPositionState();
  
}

/**
 * Wiggle the buggy left and right (random amounts) untill the front sensors have different values
 */
bool wiggleLeft = 1; // 0 = right
unsigned int wiggleDelay = 100; // 100ms = 0.1s
unsigned int postWiggleDelay = 1000; // 1s
bool SensorControl::wiggleBuggy(){
  isWiggling = true;
  Serial.println("Wiggling buggy");
  if(wiggleLeft == 1){
    // Turn Left
    m->turnLeft();
    delay(wiggleDelay);
    Movement::stopMovement();
    delay(postWiggleDelay); // Allow things to settle
    wiggleLeft = 0; // Toggle
  }else{
    // Turn right
    m->turnRight();
    delay(wiggleDelay);
    Movement::stopMovement();
    delay(postWiggleDelay); // Allow things to settle
    wiggleLeft = 1; // Toggle
  }

  /*
   * Check if the front sensors have now changed. If not rerun the function
   */
  getFrontSensorColours();  
  initialSensorReading_left         = frontSensorColours[0];
  initialSensorReading_right        = frontSensorColours[1];
  initialSensorReading_rightBackup  = frontSensorColours[2];

  if(initialSensorReading_left == initialSensorReading_right){
    wiggleDelay += 50; // Increase the wiggle duration by 0.05s
    return wiggleBuggy(); // Wiggle buggy in opposite direction
  }else{
    Serial.println("Finished wiggling");
    isWiggling = false;
    wiggleDelay = 100; // Reset wiggle delay and finish
    return true;
  }
}

bool SensorControl::isValidColour(colour c){
  if(c == WHITE || c == BLACK){
    return true;
  }else{
    return false;
  }
}

/**
 * This should be called at the start of the movement sequence
 * Get the current position of the buggy WHITE_BLACK | BLACK_WHITE
 */
void SensorControl::getStartPosition(){
  getSideSensorColours();
  initialSideSensorReading_left   = sideSensorColours[0];
  initialSideSensorReading_right  = sideSensorColours[1];

  getFrontSensorColours();  
  initialSensorReading_left  = frontSensorColours[0];
  initialSensorReading_right = frontSensorColours[1];  

  Serial.print("---- Left front sensor: ");
  Serial.println(colourToString(initialSensorReading_left));
  Serial.print("---- Right front sensor: ");
  Serial.println(colourToString(initialSensorReading_right));   
  Serial.print("---- Left side sensor: ");
  Serial.println(colourToString(initialSideSensorReading_left));
  Serial.print("---- Right side sensor: ");
  Serial.println(colourToString(initialSideSensorReading_right));  
  

  /*
   * Get the colour state
   */
  if(initialSideSensorReading_left == WHITE && initialSideSensorReading_right == BLACK){
      Serial.println("Start colour state = WHITE_BLACK");
      currentColourState = WHITE_BLACK; 
  }else if(initialSideSensorReading_left == BLACK && initialSideSensorReading_right == WHITE){
      Serial.println("Start colour state = BLACK_WHITE");
      currentColourState = BLACK_WHITE; 
  }else{
    Serial.println("Side sensors have the same value...");
    Serial.println("Please adjust the buggy and start again");
    while(true){
      ; // Halting
    }
  }

  getPositionState();  
}

void SensorControl::getPositionState(){
  /* 
   * Get the position state 
   */

  /*
   * Calculate the state number from the colours
   */
  this->positionState = (colourToNumber(initialSensorReading_left) * 8) + (colourToNumber(initialSensorReading_right) * 4) + (colourToNumber(initialSideSensorReading_left) * 2) + (colourToNumber(initialSideSensorReading_right) * 1);

  /*
   * See Notes
   */
  switch(this->positionState){
    case 4: // 0100
      // Front sensors read WHITE_BLACK and the back sensors are on WHITE. CROSS. Deviating left
      Serial.println("STATE 4");
      Serial.println("Starting on a cross, deviating left with front sensors on WHITE_BLACK and back sensors on WHITE");
      initialSideSensorReading_left  = BLACK;
      initialSideSensorReading_right = WHITE;
      currentColourState = BLACK_WHITE;
      currentPositionState = CROSS;
      break;

    case 5: // 0101
      // Ideal line. WHITE_BLACK
      Serial.println("STATE 5");
      Serial.println("Ideal line WHITE_BLACK");
      initialSideSensorReading_left  = WHITE;
      initialSideSensorReading_right = BLACK;
      currentColourState = WHITE_BLACK;
      currentPositionState = LINE;
      break;

    case 6: // 0110
      // Ideal cross. Front WHITE_BLACK, Back BLACK_WHITE
      Serial.println("STATE 6");
      Serial.println("Ideal cross with front sensors on WHITE_BLACK and back sensors on BLACK_WHITE");
      initialSideSensorReading_left  = BLACK;
      initialSideSensorReading_right = WHITE;
      currentColourState = BLACK_WHITE;
      currentPositionState = CROSS;
      break;

    case 7: // 0111
      // Front sensors read WHITE_BLACK and the back sensors are on BLACK. CROSS. Deviating right
      Serial.println("STATE 7");
      Serial.println("Starting on a cross, deviating right with front sensors on WHITE_BLACK and back sensors on BLACK");
      initialSideSensorReading_left  = BLACK;
      initialSideSensorReading_right = WHITE;
      currentColourState = BLACK_WHITE;
      currentPositionState = CROSS;
      break;

    case 8: // 1000
      // Front sensors read BLACK_WHITE and the back sensors are on WHITE. CROSS. Deviating right
      Serial.println("STATE 8");
      Serial.println("Starting on a cross, deviating right with front sensors on BLACK_WHITE and back sensors on WHITE");
      initialSideSensorReading_left  = WHITE;
      initialSideSensorReading_right = BLACK;
      currentColourState = WHITE_BLACK;
      currentPositionState = CROSS;
      break;

    case 9: // 1001
      // Ideal cross. Front BLACK_WHITE, Back WHITE_BLACK
      Serial.println("STATE 9");
      Serial.println("Ideal cross with front sensors on BLACK_WHITE and back sensors on WHITE_BLACK");
      initialSideSensorReading_left  = WHITE;
      initialSideSensorReading_right = BLACK;
      currentColourState = WHITE_BLACK;
      currentPositionState = CROSS;
      break;

    case 10: // 1010
      // Ideal line. BLACK_WHITE
      Serial.println("STATE 10");
      Serial.println("Ideal line BLACK_WHITE");
      initialSideSensorReading_left  = BLACK;
      initialSideSensorReading_right = WHITE;
      currentColourState = BLACK_WHITE;
      currentPositionState = LINE;
      break;

    case 11: // 1011
      // Front sensors read BLACK_WHITE and the back sensors are on BLACK. CROSS. Deviating left
      Serial.println("STATE 11");
      Serial.println("Starting on a cross, deviating left with front sensors on BLACK_WHITE and back sensors on BLACK");
      initialSideSensorReading_left  = WHITE;
      initialSideSensorReading_right = BLACK;
      currentColourState = WHITE_BLACK;
      currentPositionState = CROSS;
      break;

    default:
      Serial.println("STATE 0-3 or 12-15");
      Serial.println("Front sensors have the same value");
      Serial.println("Unknown position state. Halting...");
      while(true){
        ; // Halt
      }
  }

}

unsigned int SensorControl::colourToNumber(colour c){
  if(c == WHITE){
    return 0;
  }else{
    return 1;
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
bool fr = false, fl = false, sr = false, sl = false;
int colourTransitionCount = 0;

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

  if(right_changed == true){
    fr = true;
  }

  if(left_changed == true){
    fl = true;
  }
  
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


      /*
       * The IR sensors should see the same colour for all movement
       *  Determine whether the sensors are now reading different values
       */  
  
      // Left sensor has changed but right sensor has not
      if(right_changed && !left_changed && currentDeviatingState != RIGHT_NOT_CHANGED){
        // Need to turn slightly to the RIGHT to compensate
        // delay(50); // To allow for misalignment of front sensors eg one sensor sees next grid first

        if(currentDeviatingState = NONE){
          currentDeviatingState = LEFT_NOT_CHANGED;
        }
        
        // As soon as the front sensors are different
        if(frontSensorColours[0] != frontSensorColours[1]){
          currentDeviatingState = CHANGED;
        }
  
        Serial.println("Deviating to the LEFT. Attempting to compensate... Right changed and left has not");
        if(Movement::currentMovement == FORWARD){

          Movement::currentMovementCompensation = COMPENSATING_RIGHT;
          
        }else if(Movement::currentMovement == BACKWARDS){
          /*
           * Opposite polarity for going backwards
           */
          Movement::currentMovementCompensation = COMPENSATING_LEFT;
           
        }
        
      // Right sensor has changed but left sensor has not      
      }else if(left_changed && !right_changed && currentDeviatingState != LEFT_NOT_CHANGED){
        // Need to turn slightly to the LEFT to compensate
        // delay(100); // To allow for misalignment of front sensors eg one sensor sees next grid first

        if(currentDeviatingState = NONE){
          currentDeviatingState = RIGHT_NOT_CHANGED;
        }

        // As soon as the front sensors are different
        if(frontSensorColours[0] != frontSensorColours[1]){
          currentDeviatingState = CHANGED;
        }
  
        Serial.println("Deviating to the RIGHT. Attempting to compensate... Left changed and right has not");
        if(Movement::currentMovement == FORWARD){
          
          Movement::currentMovementCompensation = COMPENSATING_LEFT;
  
        }else if(Movement::currentMovement == BACKWARDS){
          /*
           * Opposite polarity for going backwards
           */
           Movement::currentMovementCompensation = COMPENSATING_RIGHT;
          
        }
  
      }else{
        // On track, so keep both motors enabled
        Movement::currentMovementCompensation = ON_TRACK;
        currentDeviatingState = NONE;
      }

      /*
       * If both the wheel sensors have changed
       */
      if(sideLeft_changed && sideRight_changed){
        toggleColourState();
        frontSensorsFlipped = false; // Reset
        
        Movement::stopMovement();
      }
      
      break;

    case TURNING_LEFT:
      /* 
       * Stop movement when LEFT sensor has changed cell
       */

      Movement::currentMovementCompensation = ON_TRACK; // Dont compensate for turns
      getSideSensorColours();
      sideLeft_changed  = (initialSideSensorReading_left  != sideSensorColours[0]);
      sideRight_changed = (initialSideSensorReading_right != sideSensorColours[1]);   

      if(sideRight_changed == true){
        sr = true;
      }

      if(sideLeft_changed == true){
        sl = true;
      }

      /*
       * FINISHING CONDITIONS
       * If on a cross (ideal situation) just check for the front right sensor to change
       * If on a line, check if the back right sensor has changed twice
       */
      if( (currentPositionState == CROSS && sr && sl) || (currentPositionState == LINE && sr && sl) ){
          toggleColourState();
          Movement::stopMovement();

          /*
           * Reset flags
           */
          fl = false;
          fr = false;
          sl = false;
          sr = false;          
      }      
      
      break;

    
    case TURNING_RIGHT:
      /* 
       * Stop movement when RIGHT sensor has changed cell
       */

      Movement::currentMovementCompensation = ON_TRACK; // Dont compensate for turns
      getSideSensorColours();
      sideLeft_changed  = (initialSideSensorReading_left  != sideSensorColours[0]);
      sideRight_changed = (initialSideSensorReading_right != sideSensorColours[1]);   

      if(sideRight_changed == true){
        sr = true;
      }

      if(sideLeft_changed == true){
        sl = true;
      }      

      /*
       * FINISHING CONDITIONS
       * If on a cross (ideal situation) just check for the front right sensor to change
       * If on a line, check if the back right sensor has changed twice
       */
      if( (currentPositionState == CROSS && sr && sl) || (currentPositionState == LINE && sr && sl) ){
          toggleColourState();
          Movement::stopMovement();

          /*
           * Reset flags
           */
          fl = false;
          fr = false;
          sl = false;
          sr = false;  
      }
      
      break;
    
    default:
      // Can only end up here if the buggy has stopped due to an interrupt
      Serial.println("motorCorrection: interrupt triggered");
      
  } // end switch 
  
} // end func

void SensorControl::toggleColourState(){
  if(currentColourState == WHITE_BLACK){
    Serial.println("BLACK_WHITE state");
    currentColourState = BLACK_WHITE;
  }else{
    Serial.println("WHITE_BLACK state");
    currentColourState = WHITE_BLACK;
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

String SensorControl::getColourState(colourState cs){
  switch(cs){
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


