#include <iostream>

#include <Communications.h>
#include <dType.h>
#include <ImageProcessing.h>
#include <Interrogation.h>
#include <Movement.h>
#include <PathFinding.h>
#include <Sensors.h>

using namespace std;

int main() {
	// Entry point for the program
	bool isFinished = false;

	// Instantiate all the classes (submodules that we need)
	Communications *c	= new Communications();
	ImageProcessing *ip	= new ImageProcessing(c);
	Interrogation *i	= new Interrogation();
	Movement *m			= new Movement();
	PathFinding *p		= new PathFinding();
	Sensors *s			= new Sensors();

	while (!isFinished) {
		// Program main loop

		// Wait untill start command is announced or maximum timeout is exceeded
		int timeout = 0;
		while (c->isStart() == false) { // Maximum timeout is 10ms * 700 = 7s
			Sleep(10); // Wait 10ms

			if (timeout > 700) {
				// Perhaps have an LED blink to display this error?
				return 0;
			}

			timeout++;
		}

		// Read the map data
		char* map = c->getMap();

		// Process the map data
		// Generate the route etc..

		isFinished = true; // Exit loop for now
	}

	return 0;
}