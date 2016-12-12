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

void setup() {
  // put your setup code here, to run once:

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Instantiating Modules...\n");

  // Create the objects we need, DONT create more than one copy.
  uss = new UltraSonicSensor();
  m = new Movement();
  p = new PathFinding();
}


double distance;
void loop() {
  // put your main code here, to run repeatedly:

  distance = uss->getDistanceToNearestObject();

  // Delay 50ms
  delay(50);
}
