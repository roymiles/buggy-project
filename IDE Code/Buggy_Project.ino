#include "Communications.h"
#include "Movement.h"
#include "PathFinding.h"
#include "Sensors.h"

Communications *c;
Movement *m;
PathFinding *p;
Sensors *s;

void setup() {
  // put your setup code here, to run once:

  // Create the objects we need, DONT create more than one copy.
  c = new Communications();
  m = new Movement();
  p = new PathFinding();
  s = new Sensors();
}

void loop() {
  // put your main code here, to run repeatedly:
}
