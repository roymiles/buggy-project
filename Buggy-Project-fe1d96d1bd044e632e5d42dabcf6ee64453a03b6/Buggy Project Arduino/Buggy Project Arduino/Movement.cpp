#include "Movement.h"

Movement::Movement()
{
}


Movement::~Movement()
{
}

void Movement::turnLeft() {
	Movement::turn(-90);
}

void Movement::turnRight() {
	Movement::turn(90);
}
