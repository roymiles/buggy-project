/**
MazeLayout.cpp
Purpose: parse maze layout string to provide easily accessible information
on the layout of the maze.

@author J. Hawkins
@version 1.0 14/12/2016
*/

#include "MazeLayout.h"

MazeLayout::MazeLayout(uint8_t sizeX, uint8_t sizeY, string description)
{

	this->xSize = sizeX;
	this->ySize = sizeY;

	//  Assume data is false.
	this->valid = MazeLayout::isValid(description);

	printf("Running Constructor\n");

	if (this->valid)
	{
		printf("Valid\n");

		/*
		* Retrieve the data from the description and determine its length
		*/
		string data = description.substr(8, description.length() - 2);
		int length = data.length();

		this->noTargets = 7;

		/*
		* Allocate memory for x and y arrays
		*/
		this->targetX = (uint8_t*)malloc(this->noTargets);
		this->targetY = (uint8_t*)malloc(this->noTargets);
		this->targetDir = (dir*)malloc(this->noTargets);

		int k = 0, j = 0;
		string cTarget;

		/*
		* Assume with validity checked there are 7 targets.
		*/
		for (; k < 35; k += 5)
		{

			// Assign values of target direction and X,Y location to grid.
			switch (cTarget.at(0)) {
			case 'A':
			case 'a':
				this->targetDir[j] = UP;
				break;
			case 'B':
			case 'b':
				this->targetDir[j] = RIGHT;
				break;
			case 'C':
			case 'c':
				this->targetDir[j] = DOWN;
				break;
			case 'D':
			case 'd':
				this->targetDir[j] = RIGHT;
			}

			this->targetX[j] = (cTarget.at(1) - '0') * 10 + (cTarget.at(2) - '0');
			this->targetY[j] = (cTarget.at(3) - '0') * 10 + (cTarget.at(4) - '0');

			j++;

		}

		// Determine number of blocked squares
		this->noBlockers = (length - 35) / 5;

		// Allocate memory for x and y arrays
		this->blockerX = (uint8_t*)malloc(this->noBlockers);
		this->blockerY = (uint8_t*)malloc(this->noBlockers);

		j = 0;
		for (; k < length; k += 5)
		{

			// Strip the current target from the string and assign values to blocker.
			cTarget = data.substr(k, k + 5);
			this->blockerX[j] = (cTarget.at(1) - '0') * 10 + (cTarget.at(2) - '0');
			this->blockerY[j] = (cTarget.at(3) - '0') * 10 + (cTarget.at(4) - '0');

			j++;

		}

		printf("Filling Result [%d]\n", this->fillBlockedPairs());

	}

}

/*
* Deconstructor, clear memory
*/
MazeLayout::~MazeLayout()
{

	// Free target allocations
	free(this->targetX);
	free(this->targetY);
	free(this->targetDir);

	// Free blocker allocations
	free(this->blockerX);
	free(this->blockerY);

}

int MazeLayout::fillBlockedPairs() {

	if (this->noBlockers % 2 == 0) {

		// Loop through each pair of blockers and fill in grid inbetween.
		// Include basic degree of error checking.

		int8_t cx, cy, dx, dy, step;

		for (int x = 0; x < MAZE_X_MAX; x++) {
			for (int y = 0; y < MAZE_Y_MAX; y++) {
				this->blockingMap[x][y] = 0;
			}
		}

		for (int k = 0; k < this->noBlockers; k += 2) {

			cx = this->blockerX[k];
			cy = this->blockerY[k];
			dx = this->blockerX[k + 1];
			dy = this->blockerY[k + 1];

			dx = dx - cx;
			dy = dy - cy;

			// Deal with case where one of blockers positions is on the grid.


			// Online continue if blocker is placed horizontally or vertically
			// i.e. (dx == 0 && dy != 0) || (dy == 0 && dx != 0)

			printf("[%d,%d]->[%d,%d]\n", cx, cy, dx, dy);

			if (dx == 0 && dy > 0) {
				// Vertical

				for (int ly = cy; ly <= cy + dy; ly++) {
					printf("[%d,%d]\n", cx, ly);
					this->blockingMap[cx][ly] = 1;
				}

			}
			else if (dx == 0 && dy < 0) {

				// Vertical (Reverse)
				for (int ly = cy; ly >= cy + dy; ly--) {
					printf("[%d,%d]\n", cx, ly);
					this->blockingMap[cx][ly] = 1;
				}

			}
			else if (dx > 0 && dy == 0) {
				// Horizontal

				for (int lx = cx; lx <= cx + dx; lx++) {
					printf("[%d,%d]\n", lx, cy);
					this->blockingMap[lx][cy] = 1;
				}

			}
			else if (dx < 0 && dy == 0) {
				// Horizontal

				for (int lx = cx; lx >= cx + dx; lx--) {
					printf("[%d,%d]\n", lx, cy);
					this->blockingMap[lx][cy] = 1;
				}

			}
			else {

				if (cx > -1 && cx < MAZE_X_MAX && cy > -1 && cy < MAZE_Y_MAX) {
					this->blockingMap[cx][cy] = 1;
				}

			}

		}

		return 0;

	}
	else {
		return -2;
	}

}

int MazeLayout::getNumberOfTargets()
{
	return this->noTargets;
}

/*
* Get the nth target X position
*/
int MazeLayout::getTargetX(int idx)
{
	if (idx < this->noTargets)
		return this->targetX[idx];
	else
		return -1;
}

/*
* Get the nth target Y position
*/
int MazeLayout::getTargetY(int idx)
{
	if (idx < this->noTargets)
		return this->targetY[idx];
	else
		return -1;
}

dir MazeLayout::getTargetDir(int idx)
{

	if (idx < this->noTargets)
		return this->targetDir[idx];
	else
		return NONE;

}

/*
* Is the grid blocked at position (x,y)
*/
bool MazeLayout::blockedAt(int x, int y)
{

	return (blockingMap[x][y] == 1);

}

// Non-static
bool MazeLayout::isValid()
{
	return this->valid;
}

// Static
bool MazeLayout::isValid(string description)
{
	// Check the preamble and the tail
	if (description.substr(0, 8).compare("*#*#*#*#") && description.substr(description.length() - 2).compare("*#"))
	{

		//        printf("\tStarts with okay\n");
		int length = description.length();
		string stripped = description.substr(8, length - 2);

		// Reset length for stripped text
		length = length - 10;

		// Expecting at least 7 targets
		if (length < 35)
		{
			//            printf("\tLength less than 35\n");
			return false;
		}

		string cTarget;
		// Check the targets are valid form
		int k;
		for (k = 0; k < 35; k += 5)
		{
			cTarget = stripped.substr(k, k + 5);
			//           printf("\tcTarget Length: %d\n", cTarget.length());
			if (!MazeLayout::isValidTarget(cTarget, false)) {
				//               printf("\tNot Valid Target\n");
				return false;
			}
		}

		// Remaining length should be multiple of 5
		//        printf("\tCLength [%d]\n", length);
		if ((length) % 5 != 0) {
			//           printf("\tLength multiple of 5\n");
			return false;
		}

		for (; k < length; k += 5)
		{
			cTarget = stripped.substr(k, k + 5);
			//           printf("\t[%d]-[%d]\n",k,k+5);
			if (!MazeLayout::isValidTarget(cTarget, true)) {
				//               printf("\tNot Valid Target 2\n");
				return false;
			}
		}

		return true;

	}
	else return false;

}

bool MazeLayout::isValidTarget(string task, bool blocker)
{

	//    char temp[8];
	//    task.toCharArray(temp, 8, 0);
	//    printf("\t\tCTarget [%s]\n",temp);
	char c = task.at(0);

	// Are we checking a target or a blocking box?
	if (!blocker)
	{
		// First char to be 'A','B','C','D' or lowercase
		//        printf("\t\tFC [%d] vs [%d]\n", c, 'A');
		if (!(c == 'A' || c == 'a' || c == 'B' || c == 'b' || c == 'C' || c == 'c' || c == 'D' || c == 'd')) {
			//            printf("\t\tIncorrect First Character\n");
			return false;
		}
	}
	else
	{
		if (c != '*') {
			//            printf("\t\tIncorrect First Character (Blocker)\n");
			return false;
		}
	}

	// Next four numbers should be
	for (int i = 1; i < 5; i++)
	{
		c = task.at(i);
		//        printf("\t\t CChar [%c]\n", c);
		if (c < '0' || c > '9') {
			//            printf("\t\t Incorrect Number");
			return false;
		}
	}

	// Passed all tests
	//    printf("\t\tTarget Valid!\n");
	return true;

}

int MazeLayout::test()
{

	MazeLayout testMaze(MAZE_X_MAX, MAZE_Y_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#");

	printf("\n-----------------------------------------------\n");

	if (testMaze.isValid()) {

		for (int x = 0; x < MAZE_X_MAX; x++) {
			printf("%d ", x);
		}
		printf("\n");

		for (int y = 0; y < MAZE_Y_MAX; y++)
		{
			printf("%d|", y);
			for (int x = 0; x < MAZE_X_MAX; x++)
			{
				if (testMaze.blockedAt(x, y))
				{
					printf("1 ");
				}
				else
				{
					printf("0 ");
				}
			}
			printf("\n");
		}

	}
	else printf("Not valid\n");

	return 0;

}
