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

PathFinding::PathFinding(MazeLayout ml, int currentPosition[2])
{
  this->ml              = ml;
  this->currentPosition = currentPosition;
  this->currentTarget   = 1;
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
unsigned int[2] PathFinding::getDestination()
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

typedef struct {
    int   distance;
    bool  visited;
} node_t;

// Data structure for all the nodes
node_t[xSize][ySize] nodes;

/*
 * Return an array of the movements required to reach the destination
 */
direction[10] dijkstras()
{
  unsigned int[2] destination  = this->getDestination();
  unsigned int xSize           = this->ml->xSize;
  unsigned int ySize           = this->ml->ySize;
  unsigned int[2] workingCell  = this->currentPosition;

  unsigned int[4][2] surroundingCells;
  
  // Initialise the nodes
  for(int i = 0; i < xSize; i++){
    for(int j = 0; j < ySize; j++){
      if(ml->blockedAt(i, j)){
        // Set all the blocked cells to visited
        nodes[i, j].visited = true; 
      }else{
        // Cell is not blocked
        nodes[i, j].visited = false; 
      }

      // All nodes are initialised with a distance 0
      nodes[i, j].distance = 0;
    }
  }

  // Set the source to visited
  nodes[workingCell[X], workingCell[Y]].visited = true;

  // Keep iterrating untill the algorithm is finished
  while(!isFinished())
  {
    /*
     * Loop through all the surrounding cells of the working cell
     */
    surroundingCells = this->adjacentCells(workingCell);
    for(int i = 0; i < 4; i++){
  
      // Only increment the distance matrix if the cell is not blocked
      if(!ml->blockedAt(surroundingCells[i][X], surroundingCells[i][Y])){ 
        // Increment the distance matrix
        distances[surroundingCells[i][X], surroundingCells[i][Y]] = distances[workingCell[X], workingCell[Y]]++;      
      }
    }

    // Change the working cell to the next (non-blocked) cell
    workingCell = nextWorkingCell();
  }
    
  
}

/*
 * Return the coordinates of the surrounding cells
 */
int[4][2] adjacentCells(int[2] position)
{
  int[2] up    = [position[X], position[Y]++];
  int[2] down  = [position[X], position[Y]--];
  int[2] left  = [position[X]--, position[Y]];
  int[2] right = [position[X]++, position[Y]];

  int[4][2] = [above, below, left, right];
}

/*
 * Return the next working cell
 */
int[2] nextWorkingCell()
{
  // TODO 
}

/*
 * Determine is algorithm has finished based on the state of the nodes
 */
bool isFinished()
{
  // TODO 
}



