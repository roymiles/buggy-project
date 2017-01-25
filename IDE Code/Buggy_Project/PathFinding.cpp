/**
    PathFinding.cpp
    Purpose: This is a submodule that will be used to
			 calculate the movement required to get the buggy
			 from A to B (and alligned appropriately)

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "PathFinding.h"

PathFinding::PathFinding(int currentPosition[2])
{
  //this->ml              = ml;
  //this->currentPosition = &currentPosition;
  //this->currentTarget   = 1;
}


PathFinding::~PathFinding()
{
}

/*
 * Return the final destination based on the target position and orientation
 */
PathFinding::Vector PathFinding::getDestination()
{
  int destination[2];
  destination[X] = MazeLayout::getTargetX(currentTarget);
  destination[Y] = MazeLayout::getTargetY(currentTarget);

  /*
   * Adjust the final destination for the buggy based on the orientation
   */
  switch(MazeLayout::getTargetOrientation(currentTarget)){
    case MazeLayout::UP:
      destination[Y]++;
      break;
    case MazeLayout::DOWN:
      destination[Y]--;
      break;
    case MazeLayout::LEFT:
      destination[X]--;
      break;
    case MazeLayout::RIGHT:
      destination[X]++;
      break;
  }

  return PathFinding::Vector(destination[X], destination[Y]);
}

/*
 * Return an array of the movements required to reach the destination
 */
MazeLayout::dir* PathFinding::dijkstras()
{
  Vector destination           = this->getDestination();
  unsigned int xSize           = MazeLayout::xSize;
  unsigned int ySize           = MazeLayout::ySize;
  Vector workingCell           = this->currentPosition;

  // Data structure for all the nodes
  node_t nodes[xSize][ySize];

  PathFinding::Vector* surroundingCells;
  
  // Initialise the nodes
  for(int i = 0; i < xSize; i++){
    for(int j = 0; j < ySize; j++){
      if(MazeLayout::blockedAt(i, j)){
        // Set all the blocked cells to visited
        nodes[i][j].visited = true; 
      }else{
        // Cell is not blocked
        nodes[i][j].visited = false; 
      }

      // All nodes are initialised with a distance 0
      nodes[i][j].distance = 0;
    }
  }

  // Set the source to visited
  nodes[workingCell.x][workingCell.y].visited = true;

  // Keep iterrating untill the algorithm is finished
  while(!isFinished())
  {
    /*
     * Loop through all the surrounding cells of the working cell
     */
    unsigned int pos[2] = {workingCell.x, workingCell.y};
    surroundingCells = this->adjacentCells(pos);
    for(int i = 0; i < 4; i++){
  
      // Only increment the distance matrix if the cell is not blocked
      if(!MazeLayout::blockedAt(surroundingCells[i].x, surroundingCells[i].y)){ 
        // Increment the distance matrix
        nodes[surroundingCells[i].x][surroundingCells[i].y].distance = nodes[workingCell.x][workingCell.y].distance++;      
      }
    }

    // Change the working cell to the next (non-blocked) cell
    workingCell = nextWorkingCell();
  }
    
  
}

/*
 * Return the coordinates of the surrounding cells
 */

PathFinding::Vector* PathFinding::adjacentCells(unsigned int pos[2])
{
  PathFinding::Vector up(pos[X], pos[Y]++);
  PathFinding::Vector down(pos[X], pos[Y]--);
  PathFinding::Vector left(pos[X]--, pos[Y]);
  PathFinding::Vector right(pos[X]++, pos[Y]);

  cells[0] = up;
  cells[1] = down;
  cells[2] = left;
  cells[3] = right;
  
  return cells;
}

/*
 * Return the next working cell
 */
PathFinding::Vector PathFinding::nextWorkingCell()
{
  // TODO 
}

/*
 * Determine is algorithm has finished based on the state of the nodes
 */
bool PathFinding::isFinished()
{
  // TODO 
}



