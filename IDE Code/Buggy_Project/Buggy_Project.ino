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
//#include "PathFinding.h"
#include "SensorControl.h"

#include "Movement.h"

//UltraSonicSensor *uss;
Ultrasonic *us;
Movement *m;
//PathFinding *p;
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

const int MAX_QUEUE_COUNT = 40; // 37
const unsigned int ULTRA_SONIC_PIN = 11;
unsigned int mvCount = 0;//22;
//movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD /* first one */, BACKWARDS, BACKWARDS, TURNING_LEFT, TURNING_LEFT, FORWARD /* second one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* third one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* fourth one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* fifth one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* sixth one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* seventh one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* eight one */
//                                           };

//movements movementQueue[1] = {FORWARD};
movements victoryRoll[6] = {FORWARD, FORWARD, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD};
movements movementQueue[MAX_QUEUE_COUNT] = {TURNING_LEFT, FORWARD};
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

void setup() {

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println(F("Program start... 5 second delay"));
  delay(5000);

  // Create the objects we need, DONT create more than one copy.
  // uss = new UltraSonicSensor();
  m    = new Movement();
  sc   = new SensorControl(m);
  us   = new Ultrasonic(ULTRA_SONIC_PIN);

  // sc->getStartPosition();
  
  // p   = new PathFinding(MAZE_X_MAX, MAZE_Y_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#", Vector(0, 0));
  // MazeLayout::test();

  // Start the I2C Bus as Master
//  Wire.begin();   
//
//  Wire.beginTransmission(SLAVE_ADDRESS);
//  Wire.write(MASTER_IDLE);                
//  Wire.endTransmission();   
}


float distance              = 20.0f;
bool isFinished             = false;
long collisionDistance      = 3; 
movements previousMovement  = FORWARD; // Always start movement with all the sensors on same squares
bool buttonPressed          = false;
bool isVictoryRoll          = false;

void loop() {
  if(buttonPressed == false){
    buttons.update();
    if(buttons.onRelease(UP_BUTTON))
    {
      //sc->movementInit(previousMovement, FORWARD);
//      m->moveForward();
//      previousMovement = FORWARD;
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
       //sc->wiggleBuggy();

       sc->movementInit(FORWARD, FORWARD);
//      sc->movementInit(previousMovement, TURNING_LEFT);
//      m->turnLeft();
//      previousMovement = TURNING_LEFT;
    }

    
    if(buttons.onRelease(DOWN_BUTTON))
    {
      /*
       * Test whether the SENSORS are working
       */
      sc->debug();
//      sc->movementInit(previousMovement, BACKWARDS);
//      m->moveBackwards();
//      previousMovement = BACKWARDS;
    }

    
    if(buttons.onRelease(RIGHT_BUTTON))
    {
      /*
       * Test whether the ULTRASONIC SENSOR is working
       */      
      us->MeasureInCentimeters();
      Serial.print("Object distance: ");
      Serial.println(us->RangeInCentimeters);
//      sc->movementInit(previousMovement, TURNING_RIGHT);
//      m->turnRight();
//      previousMovement = TURNING_RIGHT;
    }
  
    if(buttons.onRelease(STOP_BUTTON))
    {
      Serial.println("Stop button pressed");
      m->stopMovement();
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
//    Wire.beginTransmission(SLAVE_ADDRESS);
//    Wire.write(MASTER_IDLE);                
//    Wire.endTransmission(); 
 
    // Call the appropriate movement operation
    Serial.print(F("Current mvCount: "));
    Serial.println(mvCount);
    Serial.println(F("3 second delay"));
    delay(3000);

    /*
     * Botched code just for this demo
     */

     /*
      * Could be 2 or 3 FORWARD movements 
      */
//    if(mvCount == 4 || mvCount == 14 /* TESTED AND WORKING */ || mvCount == 25 || mvCount == 35){
//      // Previous colour state
//      if(SensorControl::currentColourState == BLACK_WHITE){
//        // No more forwards. Skip
//        Serial.println("SKIPPING A FORWARD");
//        mvCount++;
//      }
//    }
//
//    /* FIRST HALF TO SECOND HALF OF BOARD - TESTED*/
//    if(mvCount == 21){
//      // Previous colour state
//      if(SensorControl::currentColourState == BLACK_WHITE){
//        // No more forwards. Skip
//        Serial.println("SKIPPING A FORWARD");
//        mvCount++;
//      }
//    }    
//
//    if(mvCount == 9 || mvCount == 30){
//      // Previous colour state
//      if(SensorControl::currentColourState == BLACK_WHITE){
//        // No more forwards. Skip
//        Serial.println("SKIPPING A FORWARD");
//        mvCount++;
//      }
//    }    
    movements mv = (isVictoryRoll ? victoryRoll[mvCount] : movementQueue[mvCount]); // Read the appropriate array
    switch(movementQueue[mvCount]){
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
        m->turnLeft();
        previousMovement = TURNING_LEFT;
        break;
      /*
       * Before every turn, the buggy should be on a CROSS
       * This ensures that the rotation is close to 90 degrees
       */
      case TURNING_RIGHT:
        sc->movementInit(previousMovement, TURNING_RIGHT);
        m->turnRight();
        previousMovement = TURNING_RIGHT;
        break;
        // If IDLE do nothing

      case DOCKING:
//        Wire.beginTransmission(SLAVE_ADDRESS);
//        Wire.write(MASTER_DOCKED);                
//        Wire.endTransmission(); 
//        delay(500); // Gives slave some time to read the I2C bus
//
//        // Master thinks the buggy has docked, checking with the slave
//        Wire.requestFrom(SLAVE_ADDRESS, MASTER_DOCKED);
//        bool isDocked = false;
//        int bytes = Wire.available();
//        I2C_SLAVE response = Wire.read();
//
//        isDocked = (response == SLAVE_DOCKED ? true : false);
//        do{
//          Serial.println("Adjusting docked position");
//
//          response = Wire.read();
//          isDocked = (response == SLAVE_DOCKED ? true : false);
//          delay(500);
//        }while(isDocked == false);

        // Successfully docked
        
        break;
    }

    // Only increment, if mvCount is within the array bounds
    int count = (isVictoryRoll ? 6 : MAX_QUEUE_COUNT); // Different size if on a victory roll
    if(mvCount < count){
      // Increment the movement counter
      Serial.println("Current movement: " + m->getMovement(Movement::currentMovement));
      mvCount++;
    }else{
      Serial.println(F("Finished"));

      if(isVictoryRoll == true){
        // Finished everything
        mvCount         = 0;
        isVictoryRoll   = true;
      }else{
        // Performing the victory roll
        isFinished = true;
        Serial.println(F("Press the button to go again"));
        mvCount       = 0;
        buttonPressed = false;
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

