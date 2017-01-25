/**
    PathFinding.h
    Purpose: This is a submodule that will be used to
			 calculate the movement required to get the buggy
			 from A to B (and alligned appropriately)

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef PathFinding_h
#define PathFinding_h

#include "Arduino.h"
#include "MazeLayout.h"

#pragma once
class PathFinding : public MazeLayout
{
private:

  #define X   0     // X-Coordinate index
  #define Y   1     // Y-Coordinate index

  typedef struct {
      unsigned int   distance;
      bool  visited;
  } node_t;
  
  // ID of the current target of interest
  unsigned int currentTarget;
  
public:
	PathFinding(int currentPosition[2]);
	~PathFinding();

  struct Vector
  {
    Vector(unsigned int x, unsigned int y){
      this->x = x;
      this->y = y;  
    }
    Vector() {
      this->x = 0;
      this->y = 0;
    }

    unsigned int x;
    unsigned int y;
  };

  Vector currentPosition;
  
  Vector cells[4];

  Vector getDestination();
  Vector* adjacentCells(unsigned int pos[2]);
  Vector nextWorkingCell();
  MazeLayout::dir* dijkstras();
  bool isFinished();
};

#endif
