/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "UltraSonicSensor.h"
#include "Movement.h"
#include "PathFinding.h"
#include "SensorControl.h"

UltraSonicSensor *uss;
Movement *m;
PathFinding *p;
SensorControl *sc;

#define MAX_QUEUE_COUNT 100
unsigned int mvCount = 0;
movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD /* first one */, BACKWARDS, BACKWARDS, TURNING_LEFT, TURNING_LEFT, FORWARD /* second one */,
                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* third one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* fourth one */,
                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* fifth one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* sixth one */,
                                            TURNING_RIGHT, FORWARD, FORWARD, TURNING_LEFT /* seventh one */, TURNING_RIGHT, TURNING_RIGHT, FORWARD, FORWARD, FORWARD /* eight one */
                                           };

void setup() {

  uint8_t i;
  for (i = 4; i <= 7; i++) {
    pinMode(i, OUTPUT);
  }

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Program start");

  // Create the objects we need, DONT create more than one copy.
  // uss = new UltraSonicSensor();
  m    = new Movement();
  sc   = new SensorControl(m);
  
  // p   = new PathFinding(MAZE_X_MAX, MAZE_Y_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#", Vector(0, 0));
  // MazeLayout::test();
}


float distance;
bool isFinished = false;
void loop() {
  //sc->debugLoop();

  /*
     Move using the serial input WASD
  */
  if (Serial.available()) {
    char val = Serial.read();
    if (val != -1) {
      switch (val) {
        case 'w': //Move Forward
          m->moveForward();
          break;
        case 's': //Move Backward
          m->moveBackwards();
          break;
        case 'a': //Turn Left
          m->turnLeft();
          break;
        case 'd': //Turn Right
          m->turnRight();
          break;
        case 'z':
          Serial.println("Serial Working...");
          break;
        case 'x':
          m->stopMovement();
          break;
      }
    }
    else {
      m->stopMovement();
    }
  }

  /*
     Iterate through the movements in the queue
  */

  // Buggy is not doing anything, so move onto the next movement
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

