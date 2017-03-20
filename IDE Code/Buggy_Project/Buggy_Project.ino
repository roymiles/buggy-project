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

class Point
{
public:
  int x;
  int y;

  Point(int x, int y){
    this->x = x;
    this->y = y;
  }
  ~Point();

  void incrementX(){
    this->x++;
  }

  void decrementX(){
    this->x--;
  }
  
  void incrementY(){
    this->y++;
  }

  void decrementY(){
    this->y--;
  }

};

enum orientation {NORTH, EAST, SOUTH, WEST};
Point *currentCoordinates;
orientation currentOrientation = NORTH;

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
  currentCoordinates = new Point(0, 0); // Starting position

  Serial.print(F("Current orientation: "));
  Serial.println(orientationToString(currentOrientation));

  Serial.print(F("Current position: ("));
  Serial.print(currentCoordinates->x);
  Serial.print(F(" , "));
  Serial.print(currentCoordinates->y);
  Serial.println(F(" )"));
  
  /**
   * Print movementQueue array
   */
  Serial.print(F("movementQueue = { "));
  for (int i = 0; i < MAX_QUEUE_COUNT; i++) {
      Serial.print(m->getMovement(movementQueue[i]));
      Serial.print(F(" "));
  }
  Serial.println(F("}"));

  /**
   * Print victory roll array
   */
  Serial.print(F("victoryRoll = { "));
  for (int i = 0; i < VICTORY_ROLL_COUNT; i++) {
      Serial.print(m->getMovement(victoryRoll[i]));
      Serial.print(F(" "));
  }
  Serial.println(F("}"));
}


float distance              = 20.0f;
bool isFinished             = false;
long collisionDistance      = 1; 
movements previousMovement  = FORWARD; // Always start movement with all the sensors on same squares
bool buttonPressed          = false;
bool isVictoryRoll          = false;
bool isAdjusting            = false; // If the buggy is moving backwards to ensure on a CROSS before a turn

movements currentMovement; // Recieved from the basestation

void loop() {
  if(buttonPressed == false){
    buttons.update();
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

  /*
   * Check for a collision
   * Only do collision detection for forward movement
   */
  if(Movement::currentMovement == FORWARD){
    us->MeasureInCentimeters();
    if(us->RangeInCentimeters <= collisionDistance && us->RangeInCentimeters != 0){
      Serial.println("Stopping, object in path");
      sc->consecutiveCollisions++; // Movement ended because of a collision
      previousMovement = BACKWARDS; // Because the front and back sensors will be on opposite grid colours
      Movement::stopMovement();
    }
  }
  
  /* 
   * Iterate through the movements in the queue
   */   
  if(Movement::currentMovement == IDLE && isFinished == false && buttonPressed == true){
    // Buggy is not doing anything, so move onto the next movement
    
    // Send the current coordinates to the basestation and get the next movement
    coms->sendPosition(currentCoordinates->x, currentCoordinates->y, orientationToI2C(currentOrientation));
    movements mv = I2CToMovement(coms->getNextMovement());

    Serial.println(F("BEFORE MOVEMENT"));
    Serial.print(F("Current orientation: "));
    Serial.println(orientationToString(currentOrientation));

    Serial.print(F("Current position: ("));
    Serial.print(currentCoordinates->x);
    Serial.print(F(" , "));
    Serial.print(currentCoordinates->y);
    Serial.println(F(" )"));    
 
    // Call the appropriate movement operation
    // Serial.println(F("2 second delay"));
    delay(2000);
   
    //movements mv = getNextMovement(); 
    Serial.print(F("Current movement: "));
    Serial.print(m->getMovement(mv));
    Serial.print(F(" | Current mvCount: "));
    Serial.print(mvCount);
    Serial.print(F(" | Previous movement: "));
    Serial.println(m->getMovement(previousMovement));
    
    switch(mv){
      case FORWARD:
        sc->movementInit(previousMovement, FORWARD);
        m->moveForward();
        previousMovement = FORWARD;
        break;
      case BACKWARDS:
        sc->movementInit(previousMovement, BACKWARDS);
        m->moveBackwards();
        previousMovement = BACKWARDS;
        break;

      /*
       * Before every turn, the buggy should be on a CROSS
       * This ensures that the rotation is close to 90 degrees
       */
      case TURNING_LEFT:
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
      case TURNING_RIGHT:
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

      case DOCKING:
        // Finished interrogation
        break;

      case IDLE:
          // Do nothing
          previousMovement = IDLE;
          break;
        
    }

    // Only increment, if mvCount is within the array bounds
    int count = ((isVictoryRoll==true) ? VICTORY_ROLL_COUNT : MAX_QUEUE_COUNT); // Different size if on a victory roll
    if(mvCount < count){
      // Increment the movement counter
      // Serial.println("Current movement: " + m->getMovement(Movement::currentMovement));
      if(sc->consecutiveCollisions < 2 && isAdjusting != true){ // Buggy has tried to move forward into a box, don't update the coordinates
        /*
         * Update the coordinates and position
         */
        adjustPosition(previousMovement);
        adjustOrientation(previousMovement);
      }
      
      // Dont move onto the next movement if the buggy spent this time adjusting
      if(isAdjusting == false){
        mvCount++;
      }else{
        // Dont adjust twice in a row
        isAdjusting = false;
      }
    }else{
      Serial.println(F("Finished"));

      //if(isVictoryRoll == true){
        // Finished everything
        Serial.println(F("Press the button to go again"));
        mvCount         = 0;
        isFinished      = true;
        isVictoryRoll   = false;
        buttonPressed   = false;
//      }else{
//        // Performing the victory roll
//        Serial.println(F("Starting the VICTORY ROLL"));
//        isVictoryRoll = true;
//        mvCount       = 0;
//      }
    }
  }

  /*
   * Only perform motor correction if the buggy is moving
   */
  if(Movement::currentMovement != IDLE && Movement::isWiggling == false){
    sc->motorCorrection();    
    delay(100); // Twice the timer interrupt duration
  } 

}

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
}

void adjustOrientation(movements m){
  if(m == TURNING_RIGHT){
    // Rotate 90 degrees RIGHT
    switch(currentOrientation){
      case NORTH:
        currentOrientation = EAST;
        break;
      case EAST:
        currentOrientation = SOUTH;
        break;
      case SOUTH:
        currentOrientation = WEST;
        break;
      case WEST:
        currentOrientation = NORTH;
        break;
    }
  }else if(m == TURNING_LEFT){
    // Rotate 90 degrees LEFT
    switch(currentOrientation){
      case NORTH:
        currentOrientation = WEST;
        break;
      case EAST:
        currentOrientation = NORTH;
        break;
      case SOUTH:
        currentOrientation = EAST;
        break;
      case WEST:
        currentOrientation = SOUTH;
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

