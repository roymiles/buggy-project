/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/
#include <AnalogMultiButton.h>

#include "UltraSonic.h"
#include "Movement.h"
#include "PathFinding.h"
#include "SensorControl.h"
#include "BaseStation.h"
#include "Movement.h"
#include "Communication.h"

/*
 * Instances of classes
 */
Ultrasonic *us;
Movement *m;
PathFinding *p;
SensorControl *sc;
BaseStation *bs;
Communication *coms;

/*
 * All the buggy button variables
 */
const int BUTTONS_PIN    = A0;
const int BUTTONS_TOTAL  = 5;
const int UP_BUTTON      = 0;
const int LEFT_BUTTON    = 1;
const int DOWN_BUTTON    = 2;
const int RIGHT_BUTTON   = 3;
const int STOP_BUTTON    = 4;
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0,145,329,505,743};
AnalogMultiButton buttons(BUTTONS_PIN,BUTTONS_TOTAL,BUTTONS_VALUES);

const int SLAVE_ADDRESS = 9; // Device number of the arduino used for the interrogator

const unsigned int ULTRA_SONIC_PIN = 11;
unsigned int mvCount = 0; //22;

const int VICTORY_ROLL_COUNT = 1;
movements victoryRoll[VICTORY_ROLL_COUNT] = {IDLE};

const int MAX_QUEUE_COUNT = 1; // 37
movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD};

void setup() {

  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Create the objects we need, DONT create more than one copy.
  m    = new Movement();
  sc   = new SensorControl(m);
  us   = new Ultrasonic(ULTRA_SONIC_PIN);
  coms = new Communication();

  currentOrientation = NORTH;
  currentCoordinates = new Point(3, 5); // Starting position
  Communication::setCurState(SLAVE_IDLE);
  recievedCommand = I2C_DO_NOTHING; // Don't do anything at the start

  Serial.print(F("Current orientation: "));
  Serial.println(orientationToString(currentOrientation));

  Serial.print(F("Current position: ("));
  Serial.print(currentCoordinates->x);
  Serial.print(F(" , "));
  Serial.print(currentCoordinates->y);
  Serial.println(F(" )"));
  
//  /**
//   * Print movementQueue array
//   */
//  Serial.print(F("movementQueue = { "));
//  for (int i = 0; i < MAX_QUEUE_COUNT; i++) {
//      Serial.print(m->getMovement(movementQueue[i]));
//      Serial.print(F(" "));
//  }
//  Serial.println(F("}"));
//
//  /**
//   * Print victory roll array
//   */
//  Serial.print(F("victoryRoll = { "));
//  for (int i = 0; i < VICTORY_ROLL_COUNT; i++) {
//      Serial.print(m->getMovement(victoryRoll[i]));
//      Serial.print(F(" "));
//  }
//  Serial.println(F("}"));

}

float distance              = 20.0f;
bool isFinished             = false;
long collisionDistance      = 1; 
movements previousMovement  = FORWARD; // Always start movement with all the sensors on same squares
bool buttonPressed          = false;
bool isVictoryRoll          = false;
bool isAdjusting            = false; // If the buggy is moving backwards to ensure on a CROSS before a turn

void loop() {
  if(buttonPressed == false){
    buttons.update();
    readButtons();
  }

  /*
   * Check for a collision
   * Only do collision detection for forward movement
   */
  if(Movement::currentMovement == FORWARD){
    checkCollision();
  }
  
  /* 
   * If a command has been recieved process it
   */   
  if(recievedCommand != I2C_DO_NOTHING){
    // Recieved a new command
    Serial.print("Recieved: ");
    Serial.println(coms->commandToString(recievedVal));
    
    // Load the command and then clear it
    I2C_COMMAND cmd = recievedCommand;
    recievedCommand = I2C_DO_NOTHING;
    
    // Buggy is not doing anything, so move onto the next movement

    if(sc->consecutiveCollisions < 2 && isAdjusting != true){ // Buggy has tried to move forward into a box, don't update the coordinates
      /*
       * Update the coordinates and position
       */
      adjustPosition(previousMovement);
      adjustOrientation(previousMovement);
    }   

    // Don't adjust twice in a row
    if(isAdjusting == true){
      isAdjusting = false;
    }

    Serial.println(F("BEFORE MOVEMENT"));
    Serial.print(F("Current orientation: "));
    Serial.println(orientationToString(currentOrientation));

    Serial.print(F("Current position: ("));
    Serial.print(currentCoordinates->x);
    Serial.print(F(" , "));
    Serial.print(currentCoordinates->y);
    Serial.println(F(" )"));    
   
    Serial.print(F("Current command: "));
    Serial.print(coms->commandToString(cmd));
    Serial.print(F(" | Previous movement: "));
    Serial.println(m->getMovement(previousMovement));
    
    switch(cmd){
      case I2C_FORWARD:
        Communication::setCurState(SLAVE_MOVING);
        sc->movementInit(previousMovement, FORWARD);
        m->moveForward();
        previousMovement = FORWARD;
        break;
      case I2C_BACKWARDS:
        Communication::setCurState(SLAVE_MOVING);
        sc->movementInit(previousMovement, BACKWARDS);
        m->moveBackwards();
        previousMovement = BACKWARDS;
        break;

      /*
       * Before every turn, the buggy should be on a CROSS
       * This ensures that the rotation is close to 90 degrees
       */
      case I2C_TURN_LEFT:
        Communication::setCurState(SLAVE_MOVING);
        sc->movementInit(previousMovement, TURNING_LEFT);
        if(sc->currentPositionState == LINE && isAdjusting == false){
          Serial.println("Adjusting prior to turn");
          m->moveBackwards();
          previousMovement = BACKWARDS;
          isAdjusting = true;
        }else{
          m->turnLeft();
          previousMovement = TURNING_LEFT;
        }
        break;
        
      /*
       * Before every turn, the buggy should be on a CROSS
       * This ensures that the rotation is close to 90 degrees
       */
      case I2C_TURN_RIGHT:
        Communication::setCurState(SLAVE_MOVING);
        sc->movementInit(previousMovement, TURNING_RIGHT);
        if(sc->currentPositionState == LINE && isAdjusting == false){
          Serial.println("Adjusting prior to turn");
          m->moveBackwards();
          previousMovement = BACKWARDS;
          isAdjusting = true;
        }else{
          m->turnRight();
          previousMovement = TURNING_RIGHT;
        }
        break;

      case I2C_AT_TARGET:
        // Start docking
        break;

      case I2C_VICTORY:
        break;

      case I2C_DO_NOTHING:
          // Do nothing
          previousMovement = IDLE;
          break;
        
    } // end switch
    
  } // end command proccess

  /*
   * Only perform motor correction if the buggy is moving
   */
  if(Movement::currentMovement != IDLE && Movement::isWiggling == false){
    sc->motorCorrection();    
    delay(100); // Twice the timer interrupt duration
  } 

} // end loop

void checkCollision(){
  us->MeasureInCentimeters();
  if(us->RangeInCentimeters <= collisionDistance && us->RangeInCentimeters != 0){
    Serial.println("Stopping, object in path");
    sc->consecutiveCollisions++; // Movement ended because of a collision
    previousMovement = BACKWARDS; // Because the front and back sensors will be on opposite grid colours
    Movement::stopMovement();
  }
}

void readButtons(){
  if(buttons.onRelease(UP_BUTTON))
  {
    Serial.println(F("Pressed button.. Will begin in 2 seconds"));
    isFinished = false; // Start of attempt
    sc->getStartPosition();
    
    delay(2000);
    buttonPressed = true;
  }

  
  if(buttons.onRelease(LEFT_BUTTON))
  {
    /*
     * Test whether the WIGGLE is working
     */
     sc->movementInit(FORWARD, FORWARD);
  }

  
  if(buttons.onRelease(DOWN_BUTTON))
  {
    /*
     * Test whether the SENSORS are working
     */
    sc->debug();
  }

  
  if(buttons.onRelease(RIGHT_BUTTON))
  {
    /*
     * Test whether the ULTRASONIC SENSOR is working
     */      
    us->MeasureInCentimeters();
    Serial.print(F("Object distance: "));
    Serial.println(us->RangeInCentimeters);
  }

  if(buttons.onRelease(STOP_BUTTON))
  {
    /*
     * Use this button to debug whatever else
     */
    Serial.println(F("Moving forward button"));
    sc->movementInit(previousMovement, FORWARD);
    m->moveForward();
    previousMovement = FORWARD;
  } 
}

/**
 * Change the current position based on the previous movement
 */
void adjustPosition(movements m){
  if(m == FORWARD){
    switch(currentOrientation){
      case NORTH:
        currentCoordinates->incrementY();
        break;
      case EAST:
        currentCoordinates->incrementX();
        break;
      case SOUTH:
        currentCoordinates->decrementY();
        break;
      case WEST:
        currentCoordinates->decrementX();
        break;
    }
  }else if(m == BACKWARDS){
    switch(currentOrientation){
      case NORTH:
        currentCoordinates->decrementY();
        break;
      case EAST:
        currentCoordinates->decrementX();
        break;
      case SOUTH:
        currentCoordinates->incrementY();
        break;
      case WEST:
        currentCoordinates->incrementX();
        break;
    }
  }

  /*
   * Update the dataBuffer
   */
   dataBuffer[0] = (char)(currentCoordinates->x & 0xFF); 
   dataBuffer[1] = (char)(currentCoordinates->y & 0xFF); 
}

/**
 * Change the current orientation based on the previous movement
 */
void adjustOrientation(movements m){
  if(m == TURNING_RIGHT){
    // Rotate 90 degrees RIGHT
    switch(currentOrientation){
      case NORTH:
        Communication::setCurrentOrientation(EAST);
        break;
      case EAST:
        Communication::setCurrentOrientation(SOUTH);
        break;
      case SOUTH:
        Communication::setCurrentOrientation(WEST);
        break;
      case WEST:
        Communication::setCurrentOrientation(NORTH);
        break;
    }
  }else if(m == TURNING_LEFT){
    // Rotate 90 degrees LEFT
    switch(currentOrientation){
      case NORTH:
        Communication::setCurrentOrientation(WEST);
        break;
      case EAST:
        Communication::setCurrentOrientation(NORTH);
        break;
      case SOUTH:
        Communication::setCurrentOrientation(EAST);
        break;
      case WEST:
        Communication::setCurrentOrientation(SOUTH);
        break;
    }    
  }
}

String orientationToString(orientation o){
  switch(o){
    case NORTH:
      return "NORTH";
      break;
    case EAST:
      return "EAST";
      break;
    case SOUTH:
      return "SOUTH";
      break;
    case WEST:
      return "WEST";
      break;
  }
}

movements I2CToMovement(I2C_COMMAND ic){
  switch(ic){
    case I2C_FORWARD:
      return FORWARD;
      break;
    case I2C_BACKWARDS:
      return BACKWARDS;
      break;
    case I2C_TURN_LEFT:
      return TURNING_LEFT;
      break;
    case I2C_TURN_RIGHT:
      return TURNING_RIGHT;
      break;
  }
}

I2C_COMMAND orientationToI2C(orientation o){
  switch(o){
    case EAST:
      return I2C_EAST;
      break;
    case SOUTH:
      return I2C_SOUTH;
      break;
    case WEST:
      return I2C_WEST;
      break;
    case NORTH:
      return I2C_NORTH;
      break;
  }
}

movements getNextMovement(){
  /*
   * Request the next movement from the basestation
   * bs->getNextMovement(currentCoordinates.x, currentCoordinates.y);
   */
   return ((isVictoryRoll==true) ? victoryRoll[mvCount] : movementQueue[mvCount]); // Read the appropriate array
}

