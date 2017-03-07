/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/
#include <AnalogMultiButton.h>
#include <Wire.h>
//#include "UltraSonicSensor.h" REDUNDANT IMPLEMENTATION
#include "UltraSonic.h"
#include "Movement.h"
#include "PathFinding.h"
#include "SensorControl.h"

#include "Movement.h"

//UltraSonicSensor *uss;
Ultrasonic *us;
Movement *m;
PathFinding *p;
SensorControl *sc;

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

const int MAX_QUEUE_COUNT = 2; // 37
const unsigned int ULTRA_SONIC_PIN = 11;
unsigned int mvCount = 0; //22;
//movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD /* first one */, BACKWARDS, BACKWARDS, TURNING_LEFT, TURNING_LEFT, FORWARD /* second one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* third one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* fourth one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* fifth one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* sixth one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* seventh one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* eight one */
//                                           };

//movements movementQueue[1] = {FORWARD};
movements victoryRoll[6] = {FORWARD, FORWARD, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD};
movements movementQueue[MAX_QUEUE_COUNT] = {TURNING_RIGHT, TURNING_LEFT};
//movements movementQueue[MAX_QUEUE_COUNT] = {TURNING_LEFT, /* #1 */ 
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #2 */
//                                            TURNING_LEFT, FORWARD, FORWARD, FORWARD, TURNING_RIGHT, /* #1 */
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #2 */
//                                            
//                                            TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD, FORWARD, /* #3 */
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #4 */
//                                            TURNING_LEFT, FORWARD, FORWARD, FORWARD, TURNING_RIGHT, /* #3 */
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #4 */};

enum I2C_MASTER {MASTER_IDLE = 0, MASTER_MOVING = 1, MASTER_DOCKED = 2, MASTER_ADJUSTING = 3};
enum I2C_SLAVE {SLAVE_IDLE = 0, SLAVE_BUSY = 1, SLAVE_DOCKED = 2, SLAVE_NOT_DOCKED = 3};

class Point
{
public:
  signed int x;
  signed int y;

  Point(signed int x, signed int y){
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

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println(F("Program start... 2 second delay"));
  delay(2000);

  // Create the objects we need, DONT create more than one copy.
  // uss = new UltraSonicSensor();
  m    = new Movement();
  sc   = new SensorControl(m);
  us   = new Ultrasonic(ULTRA_SONIC_PIN);
  currentCoordinates = new Point(0, 0); // Starting position

  Serial.print(F("Current orientation: "));
  Serial.println(orientationToString(currentOrientation));

  Serial.print(F("Current position: ("));
  Serial.print(currentCoordinates->x);
  Serial.print(F(" , "));
  Serial.print(currentCoordinates->y);
  Serial.println(F(" )"));

  // sc->getStartPosition();
  
//  p   = new PathFinding(MAZE_X_MAX, MAZE_Y_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#", Vector(0, 0));
//  MazeLayout::test();

  // Start the I2C Bus as Master
  Wire.begin();   
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(MASTER_IDLE);                
  Wire.endTransmission();   
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
    if(buttons.onRelease(UP_BUTTON))
    {
      Serial.println(F("Pressed button.. Will begin in 2 seconds"));
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
      //Serial.println("Stop button pressed");
      //m->stopMovement();

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
    //Serial.print("Object distance: ");
    //Serial.println(us->RangeInCentimeters);
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

  /* 
   * Buggy is not doing anything, so move onto the next movement 
   */
   
//movements movementQueue[MAX_QUEUE_COUNT] = {TURNING_LEFT, /* #1  0*/ 
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #2 1-5*/
//                                            TURNING_LEFT, FORWARD, FORWARD, FORWARD, TURNING_RIGHT, /* #1 6-10*/
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #2 11-15*/
//                                            
//                                            TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD, FORWARD, /* #3 16-21*/
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #4 22-26*/
//                                            TURNING_LEFT, FORWARD, FORWARD, FORWARD, TURNING_RIGHT, /* #3 27-31*/
//                                            TURNING_RIGHT, FORWARD, FORWARD, FORWARD, TURNING_LEFT, /* #4 32-36*/};
   
  if(Movement::currentMovement == IDLE && isFinished == false && buttonPressed == true){
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(MASTER_IDLE);                
    Wire.endTransmission(); 

      Serial.println(F("BEFORE MOVEMENT"));
      Serial.print(F("Current orientation: "));
      Serial.println(orientationToString(currentOrientation));

      Serial.print(F("Current position: ("));
      Serial.print(currentCoordinates->x);
      Serial.print(F(" , "));
      Serial.print(currentCoordinates->y);
      Serial.println(F(" )"));    
 
    // Call the appropriate movement operation
    Serial.print(F("Current mvCount: "));
    Serial.println(mvCount);
    // Serial.println(F("3 second delay"));
    delay(3000);
   
    movements mv = getNextMovement();
    Serial.print(F("Current movement: "));
    Serial.println(m->getMovement(mv));
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
          m->moveBackwards();
          previousMovement = BACKWARDS;
          isAdjusting = true;
        }else{
          m->turnRight();
          previousMovement = TURNING_RIGHT;
        }
        break;
        // If IDLE do nothing

      case DOCKING:
        Wire.beginTransmission(SLAVE_ADDRESS);
        Wire.write(MASTER_DOCKED);                
        Wire.endTransmission(); 
        delay(500); // Gives slave some time to read the I2C bus

        /*
         * Wiggle the buggy, while checking if the buggy has docked
         */
        sc->wiggleBuggy(previousMovement, true);

        // Successfully docked
        I2C_SLAVE response = SLAVE_BUSY;
        while(response == SLAVE_BUSY){
          // Wait for slave to be done
          Wire.requestFrom(SLAVE_ADDRESS, SLAVE_BUSY);
          bool isBusy = false;
          int bytes = Wire.available();
          I2C_SLAVE response = Wire.read(); 
          delay(500);         
        }

        // Finished interrogation
        
        break;
    }

    // Only increment, if mvCount is within the array bounds
    int count = ((isVictoryRoll==true) ? 6 : MAX_QUEUE_COUNT); // Different size if on a victory roll
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

      if(isVictoryRoll == true){
        // Finished everything
        Serial.println(F("Press the button to go again"));
        mvCount         = 0;
        isFinished      = true;
        isVictoryRoll   = false;
        buttonPressed   = false;
      }else{
        // Performing the victory roll
        Serial.println(F("Starting the VICTORY ROLL"));
        isVictoryRoll = true;
        mvCount       = 0;
      }
    }
  }

  /*
   * Only perform motor correction if the buggy is moving
   */
  if(Movement::currentMovement != IDLE && Movement::isWiggling == false){
    sc->motorCorrection();
    
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(MASTER_MOVING);                
    Wire.endTransmission(); 
    
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

movements getNextMovement(){
  /*
   * Request the next movement from the basestation
   * bs->getNextMovement(currentCoordinates.x, currentCoordinates.y);
   */
   return ((isVictoryRoll==true) ? victoryRoll[mvCount] : movementQueue[mvCount]); // Read the appropriate array
}

