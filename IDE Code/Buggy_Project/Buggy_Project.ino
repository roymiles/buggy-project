/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/
#include <AnalogMultiButton.h>
//#include "UltraSonicSensor.h"
#include "UltraSonic.h"
#include "Movement.h"
#include "PathFinding.h"
#include "SensorControl.h"

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

#define MAX_QUEUE_COUNT 100
#define ULTRA_SONIC_PIN 11
unsigned int mvCount = 0;
movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD /* first one */, BACKWARDS, BACKWARDS, TURNING_LEFT, TURNING_LEFT, FORWARD /* second one */,
                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* third one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* fourth one */,
                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* fifth one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* sixth one */,
                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* seventh one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* eight one */
                                           };

void setup() {

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(5000);
  Serial.println("Program start");

  // Create the objects we need, DONT create more than one copy.
  // uss = new UltraSonicSensor();
  m    = new Movement();
  sc   = new SensorControl(m);
  us   = new Ultrasonic(ULTRA_SONIC_PIN);
  
  // p   = new PathFinding(MAZE_X_MAX, MAZE_Y_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#", Vector(0, 0));
  // MazeLayout::test();
}


float distance;
bool isFinished = false;
long collisionDistance = 8; 

void loop() {
  /*buttons.update();
  if(buttons.onRelease(UP_BUTTON))
  {
    Serial.println("Up button pressed");
    sc->movementInit();
    m->moveForward();
  }

  if(buttons.onRelease(LEFT_BUTTON))
  {
    Serial.println("Left button pressed");
    sc->movementInit();
    m->turnLeft();
  }

  if(buttons.onRelease(DOWN_BUTTON))
  {
    Serial.println("Down button pressed");
    sc->movementInit();
    m->moveBackwards();
  }

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
  } 

  if(Movement::currentMovement != IDLE){
    sc->motorCorrection();
  }*/
  
  //sc->debug();
  //delay(1000);   
 //sc->movementInit();
  //delay(1000); 

  //Serial.println(sc->getColour(sc->debugColour()));
  //sc->debug();
  //delay(500);
  //sc->motorCorrection();

  /*if (Serial.available()) {
    char val = Serial.read();
    if (val != -1) {
      switch (val) {
        case 'q':
          sc->enableLeftSensor();
          break;
        case 'e':
          sc->enableRightSensor();
          break;
      }
    }
  }*/
  

  /*
     Move using the serial input WASD
  */
  if (Serial.available()) {
    char val = Serial.read();
    if (val != -1) {
      switch (val) {
        case 'w': //Move Forward
          if(sc->movementInit()){
            m->moveForward();
          }
          break;
        case 's': //Move Backward
          if(sc->movementInit()){
            m->moveBackwards();
          }
          break;
        case 'a': //Turn Left
          if(sc->movementInit()){
            m->turnLeft();
          }
          break;
        case 'd': //Turn Right
          if(sc->movementInit()){
            m->turnRight();
          }
          break;
        case 'z':
          Serial.println("Serial Working...");
          break;
        case 'x':
          Movement::stopMovement();
          break;
        case 'q':
          //sc->enableLeftSensor();
          break;
        case 'e':
          //sc->enableRightSensor();
          break;
      }
    }
    else {
      Movement::stopMovement();
    }
  }

  /*us->MeasureInCentimeters();
  Serial.print("Object distance: ");
  Serial.println(us->RangeInCentimeters);
  delay(500);*/
  
  if(Movement::currentMovement != IDLE){
    sc->motorCorrection();
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
      Serial.println("Object in path");
      //wwMovement::stopMovement();
    }
  }
  
  /*
   * Iterate through the movements in the queue
   */

  /* 
   * Buggy is not doing anything, so move onto the next movement 
   */
  /*if(Movement::currentMovement == IDLE && isFinished == false){ //&& buttonPressed == true
    // Call the appropriate movement operation
    switch(movementQueue[mvCount]){
      case FORWARD:
        m->moveForward();
        delay(100);
        break;
      case BACKWARDS:
        m->moveBackwards();
        delay(100);
        break;
      case TURNING_LEFT:
        m->turnLeft();
        delay(100);
        break;
      case TURNING_RIGHT:
        m->turnRight();
        delay(100);
        break;
    }

    // Only increment, if mvCount is within the array bounds
    if(mvCount < MAX_QUEUE_COUNT){
      // Increment the movement counter
      Serial.println("Incrementing movement counter");
      Serial.println("Current movement: " + m->getMovement(Movement::currentMovement));
      mvCount++;
    }else{
      Serial.println("Finished");
      isFinished = true;
    }
    }*/

}

