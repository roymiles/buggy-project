/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/
#include <AnalogMultiButton.h>
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

#define MAX_QUEUE_COUNT 7
const unsigned int ULTRA_SONIC_PIN = 11;
unsigned int mvCount = 0;
//movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD /* first one */, BACKWARDS, BACKWARDS, TURNING_LEFT, TURNING_LEFT, FORWARD /* second one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* third one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* fourth one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* fifth one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* sixth one */,
//                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* seventh one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* eight one */
//                                           };

movements movementQueue[MAX_QUEUE_COUNT] = {TURNING_RIGHT};

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
}


float distance              = 20;
bool isFinished             = false;
long collisionDistance      = 1; 
movements previousMovement  = FORWARD; // Always start movement with all the sensors on same squares
bool buttonPressed          = false;

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
       * Test whether the wiggle is working
       */
       //sc->wiggleBuggy();

       sc->movementInit(FORWARD, FORWARD);
    }

    
    if(buttons.onRelease(DOWN_BUTTON))
    {
      /*
       * Test whether the sensors are working
       */
      sc->debug();
    }

    /*
    if(buttons.onRelease(RIGHT_BUTTON))
    {
      Serial.println("Right button pressed");
      sc->movementInit();
      m->turnRight();
    }
  
    if(buttons.onRelease(STOP_BUTTON))
    {
      Serial.println("Stop button pressed");
      m->stopMovement();
    } */
  }

  /*
   * Check for a collision
   * Only do collision detection for forward movement
   */
  /*if(Movement::currentMovement == FORWARD){
    us->MeasureInCentimeters();
    //Serial.print("Object distance: ");
    //Serial.println(us->RangeInCentimeters);
    if(us->RangeInCentimeters <= collisionDistance && us->RangeInCentimeters != 0){
      Serial.println("Stopping, object in path");
      previousMovement = BACKWARDS; // Because the front and back sensors will be on opposite grid colours
      Movement::stopMovement();
    }
  }*/
  
  /*
   * Iterate through the movements in the queue
   */

  /* 
   * Buggy is not doing anything, so move onto the next movement 
   */
  if(Movement::currentMovement == IDLE && isFinished == false && buttonPressed == true){
    // Call the appropriate movement operation
    Serial.println(F("3 second delay"));
    delay(3000);
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
    }

    // Only increment, if mvCount is within the array bounds
    if(mvCount < MAX_QUEUE_COUNT){
      // Increment the movement counter
      Serial.println("Current movement: " + m->getMovement(Movement::currentMovement));
      mvCount++;
    }else{
      Serial.println(F("Finished"));
      // isFinished = true;
      Serial.println(F("Press the button to go again"));
      mvCount       = 0;
      buttonPressed = false;
    }
  }

  /*
   * Only perform motor correction if the buggy is moving
   */
  if(Movement::currentMovement != IDLE && isWiggling == false){
    sc->motorCorrection();
    delay(200); // Twice the timer interrupt duration
  } 

}

