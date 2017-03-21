#include <iostream>

#include <Communications.h>
#include <Debug.h>
#include <dType.h>
#include <ImageProcessing.h>
#include <Interrogation.h>
#include <Movement.h>
#include <PathFinding.h>
#include <Sensors.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	// Entry point for the program
	bool isFinished = false;

	// Instantiate all the classes (submodules that we need)
	Debug *d			= new Debug();
	Communications *c	= new Communications(d);
	ImageProcessing *ip	= new ImageProcessing();
	Interrogation *i	= new Interrogation();
	Movement *m			= new Movement();
	PathFinding *p		= new PathFinding();
	Sensors *s			= new Sensors();

	Mat image;
	image = ip->getImage();

	image = ip->convertToBW(image);

	double angle = ip->getAngle(image);

	//ip->test();

	// Wait untill start command is announced or maximum timeout is exceeded
	/*int timeout = 0;
	d->log("Waiting for start...");
	while (c->isStart() == false) { // Maximum timeout is 10ms * 700 = 7s
		Sleep(10); // Wait 10ms

		if (timeout > 700) {
			d->error("Failed to recieve start command before timing out");
			return 0;
		}

		timeout++;
	}

	// Received the start command

	// Read the map data
	char* map = c->getMap();

	// Process the map data (josh)
	map = m->processMap();

	// Generate the route from A->B
	allMovements[] = m->getMovements(map);

	// Loop through the movements
	for (movement in allMovements) {
		// eg m->moveFoward(); m->moveLeft
		switch (movement) {
			case FORWARD:
				m->moveForward();
				break;
		}
	}*/



	return 0;
}