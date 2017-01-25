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
//MazeLayout *ml;

void setup() {
  // put your setup code here, to run once:
  //interrupts();

  int i;
  for(i=4;i<=7;i++){
    pinMode(i, OUTPUT);  
  }

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Run keyboard control");
  
  Serial.print("Instantiating Modules...\n");

  // Create the objects we need, DONT create more than one copy.
  uss = new UltraSonicSensor();
  m   = new Movement();
  //ml  = new MazeLayout("njnjn");

  // PathFinding takes the MazeLayout object and the initial position of the buggy
  //p   = new PathFinding(ml, {0, 0});
  //MazeLayout::test();
}


double distance;
void loop() {
  // put your main code here, to run repeatedly:
  m->Test();
  //distance = uss->getDistanceToNearestObject();

  // Delay 50ms
  //delay(50);

  if(Serial.available()) {
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
          Serial.println("Hello");
          break;
        case 'x':
          m->stopMovement();
          break;
      }
    }
    else{
      m->stopMovement();  
    }
  }

  
  
}
