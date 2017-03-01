/**
    PathFinding.cpp
    Purpose: This is a submodule that will be used to
			 calculate the movement required to get the buggy
			 from A to B (and alligned appropriately)

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "PathFinding.h"

PathFinding::PathFinding(uint8_t sizeX, uint8_t sizeY, String description, Vector currentPosition) : MazeLayout(sizeX, sizeY, description)
{
  this->currentPosition = currentPosition;
  this->currentTarget   = 1;
}


PathFinding::~PathFinding()
{
}

/*
 * Return the final destination based on the target position and orientation
 */
Vector PathFinding::getDestination()
{
  int destination[2];
  destination[X] = MazeLayout::getTargetX(currentTarget);
  destination[Y] = MazeLayout::getTargetY(currentTarget);

  /*
   * Adjust the final destination for the buggy based on the orientation
   */
  switch(MazeLayout::getTargetDir(currentTarget)){
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

  return Vector(destination[X], destination[Y]);
}

/*
 * Return an array of the movements required to reach the destination
 */
dir* PathFinding::dijkstras()
{
  Vector destination           = this->getDestination();
  Vector workingCell           = this->currentPosition;

  Vector* surroundingCells;
  
  // Initialise the nodes
  for(int i = 0; i < MazeLayout::xSize; i++){
    for(int j = 0; j < MazeLayout::ySize; j++){
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
    for(int i = 0; i < 4; /* there are 4 surrounding cells */ i++){
  
      // Only increment the distance matrix if the cell is not blocked
      if(!MazeLayout::blockedAt(surroundingCells[i].x, surroundingCells[i].y)){ 
        // Increment the distance matrix
        nodes[surroundingCells[i].x][surroundingCells[i].y].distance = nodes[workingCell.x][workingCell.y].distance++;      
      }
    }

    // Change the working cell to the next (non-blocked) cell
    workingCell = nextWorkingCell();

    // Set the working cell to visited
    nodes[workingCell.x][workingCell.y].visited = true;
  }

  // TODO: Traverse back from the destination to find the shortest path
  
}

/*
 * Return the coordinates of the surrounding cells
 * The only connecting nodes are those that are directly adjacent to a given cell
 * on the given grid system
 */

Vector* PathFinding::adjacentCells(unsigned int pos[2])
{
  Vector up(pos[X], pos[Y]++);
  Vector down(pos[X], pos[Y]--);
  Vector left(pos[X]--, pos[Y]);
  Vector right(pos[X]++, pos[Y]);

  cells[0] = up;
  cells[1] = down;
  cells[2] = left;
  cells[3] = right;
  
  return cells;
}

/*
 * Return the next working cell
 */
Vector PathFinding::nextWorkingCell()
{
  /*
   * Loop through all the nodes and find the node with the smallest distance and has not been seen
   */
  int lowestDistance = -1;
  Vector workingCell;
  for(int i = 0; i < MazeLayout::xSize; i++){
    for(int j = 0; j < MazeLayout::ySize; j++){

      // Has this node already been visited?
      if(nodes[i][j].visited == false){

        // If the first node that isnt visited or a node with a lower distance
        if(lowestDistance == -1 || nodes[i][j].distance < lowestDistance){
          workingCell.x  = i;
          workingCell.y  = j;
          lowestDistance = nodes[i][j].distance;
        }
        
      }
      
    }
  }

  return workingCell;
   
}

/*
 * Determine is algorithm has finished based on the state of the nodes
 */
bool PathFinding::isFinished()
{
  bool isFinished = true;
  for(int i = 0; i < MazeLayout::xSize; i++){
    for(int j = 0; j < MazeLayout::ySize; j++){
      // Has this node already been visited?
      if(nodes[i][j].visited == false){
        isFinished = false;
      }
    }
  }

  return isFinished;
}


/*
 * Convert the movement enum to a string (for debugging)
 */
String PathFinding::getDirection(dir d){
  switch(d){
    case UP:
      return "UP";
      break;
    case DOWN:
      return "DOWN";
      break;
    case LEFT:
      return "LEFT";
      break;
    case RIGHT:
      return "RIGHT";
      break;
    default:
      return "UNKNOWN";
  }
}



