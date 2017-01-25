/**
    MazeLayout.h
    Purpose: parse maze layout string to provide easily accessible information
		on the layout of the maze.

    @author J. Hawkins
    @version 1.0 14/12/2016
*/

#ifndef MazeLayout_h
#define MazeLayout_h

// Error Messages
#define MSG_INVALID_MAZE "Maze string is invalid".
#define ERR_BLOCKER_DIAGONAL -1;

#define MAZE_X_MAX 14
#define MAZE_Y_MAX 10

enum dir {NONE, UP, DOWN, LEFT, RIGHT};

#include "Arduino.h"

#pragma once
class MazeLayout
{
public:

	MazeLayout(uint8_t sizeX, uint8_t sizeY, String description);
	~MazeLayout();
	// Check whether the maze description can be parsed.
	static bool isValid(String description);
	// Is the current description valid?
	bool isValid();

    // Returns the number of targets found in the description
	int getNumberOfTargets();
    // Returns the nth target's X position
	int getTargetX(int idx);
	// Returns the nth target's Y position
	int getTargetY(int idx);
	// Returns the nth target's facing direciton (A/B/C/D).
	dir getTargetDir(int idx);
    // Is the (x,y) location blocked.
	bool blockedAt(int x, int y);
    // The full array containing 1/0 for a location blocked/unblocke.
	int blockingMap[MAZE_X_MAX][MAZE_Y_MAX];

  // Grid sizes (Needs to be public for Dijskstras algorithm in PathFinding)
  uint8_t xSize;
  uint8_t ySize;  

    // TEST FUNCTION ONLY
	static int test();

private:

	// Methods
	static bool isValidTarget(String task, bool blocker);

	int fillBlockedPairs();

	// Valid
	bool valid;

	// Keep track of the number of targets etc. for the
	// arrays below.
	int noTargets;
	int noBlockers;

	// Pointers to memory at which target x,y positions are stored
	dir* targetDir;
	uint8_t* targetX;
	uint8_t* targetY;
	uint8_t* blockerX;
	uint8_t* blockerY;

};

#endif
