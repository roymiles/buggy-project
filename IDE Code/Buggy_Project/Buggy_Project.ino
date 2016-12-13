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
const byte LRE = 0; //Left rotary encoder pin holder value
const byte RRE = 1; //Right rotary encoder pin holder value


void setup() {
  // put your setup code here, to run once:
  interrupts();

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
  attachInterrupt(digitalPinToInterrupt(LRE),m::ISRLeftEncoder, LOW);
  attachInterrupt(digitalPinToInterrupt(RRE), m::ISRRightEncoder, LOW);
}


double distance;
void loop() {
  // put your main code here, to run repeatedly:

  distance = uss->getDistanceToNearestObject();

  // Delay 50ms
  delay(50);
}
