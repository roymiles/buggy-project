/**
    PathFinding.cpp
    Purpose: This is a submodule that will be used to
			 calculate the movement required to get the buggy
			 from A to B (and alligned appropriately)

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "PathFinding.h"
#include "MazeLayout.h"

PathFinding::PathFinding(MazeLayout ml)
{
  this->ml = ml;
  this->currentTarget = 1;
}


PathFinding::~PathFinding()
{
}

#define X   0     // X-Coordinate index
#define Y   1     // Y-Coordinate index
enum direction {UP, DOWN, LEFT, RIGHT};
/*
 * Return the final destination based on the target position and orientation
 */
int[2] PathFinding::getDestination()
{
  int[2] destination;
  destination[X] = ml->getTargetX(currentTarget);
  destination[Y] = ml->getTargetY(currentTarget);

  /*
   * Adjust the final destination for the buggy based on the orientation
   */
  switch(ml->getTarget(currentTarget)){
    case UP:
      destination[Y]++;
      break;
    case DOWN:
      destination[Y]--;
      break;
    case LEFT:
      destination[X]--;
      break;
    case RIGHT:
      destination[X]++;
      break;
  }

  return destination;
}


