/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "UltraSonicSensor.h"
#include "Movement.h"
#include "PathFinding.h"

UltraSonicSensor *uss;
Movement *m;
PathFinding *p;

#define MAX_QUEUE_COUNT 10
unsigned int mvCount = 0;
movements movementQueue[MAX_QUEUE_COUNT] = {FORWARD, TURNING_RIGHT};

void setup() {

  int i;
  for(i = 4; i <= 7; i++){
    pinMode(i, OUTPUT);  
  }

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Program start");

  // Create the objects we need, DONT create more than one copy.
  uss = new UltraSonicSensor();
  m   = new Movement();
  p   = new PathFinding(MAZE_X_MAX, MAZE_Y_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#", Vector(0, 0));
  //MazeLayout::test();
}


double distance;
bool isFinished = false;
void loop() {
  //m->Test();
  //distance = uss->getDistanceToNearestObject();

  // Delay 50ms
  //delay(50);

  /*
   * Move using the serial input
   */
  /*if(Serial.available()) {
    char val = Serial.read();
    if(val != -1) {
      switch(val) {
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
    else{
      m->stopMovement();  
    }
  }*/


  /*
   * Iterrate through the movements in the queue
   */

  // Buggy is not doing anything, so move onto the next movement
  if(currentMovement == IDLE && isFinished == false){
    // Call the appropriate movement operation
    switch(movementQueue[mvCount]){
      case FORWARD:
        m->moveForward();
        break;
      case BACKWARDS:
        m->moveBackwards();
        break;
      case TURNING_LEFT:
        m->turnLeft();
        break;
      case TURNING_RIGHT:
        m->turnRight();
        break;    
    }

    // Only increment, if mvCount is within the array bounds
    if(mvCount < MAX_QUEUE_COUNT){
      // Increment the movement counter    
      Serial.println("Incremening movement counter");
      mvCount++;  
    }else{
      Serial.println("Finished");
      isFinished = true;  
    }
  } 

}
