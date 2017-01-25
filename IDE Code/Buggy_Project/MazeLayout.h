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

#include "Arduino.h"

#pragma once
class MazeLayout
{
public:
	//MazeLayout(String description);
  MazeLayout(){}
	~MazeLayout();

  enum dir {UP, DOWN, LEFT, RIGHT};
  unsigned int xSize;
  unsigned int ySize;
  
	// Check whether the maze description can be parsed.
	static bool isValid(String description);
	bool isValid();

	int getNumberOfTargets();

	int getTargetX(int idx);
	int getTargetY(int idx);
  dir getTargetOrientation(int idx);

	bool blockedAt(int x, int y);

	static int test();

private:
	
	// Methods
	static bool isValidTarget(String task, bool blocker);

	// Valid
	bool valid;

	// Keep track of the number of targets etc. for the 
	// arrays below.
	int noTargets;
	int noBlockers;

	// Pointers to memory at which target x,y positions are stored
	char* targetDir;
	uint8_t* targetX;
	uint8_t* targetY;
	uint8_t* blockerX;
	uint8_t* blockerY;

};

#endif
