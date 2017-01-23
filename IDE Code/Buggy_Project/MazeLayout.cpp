/**
  MazeLayout.cpp
  Purpose: parse maze layout string to provide easily accessible information
  on the layout of the maze.

  @author J. Hawkins
  @version 1.0 14/12/2016
*/

#include "MazeLayout.h"

MazeLayout::MazeLayout(String description) {

  //  Assume data is false.
  this->valid = MazeLayout::isValid(description);

  if (this->valid) {

    /*
     * Retrieve the data from the description and determine its length
     */
    String data = description.substring(8, description.length() - 3);
    int length = data.length();

    this->noTargets = 7;

    /* 
     * Allocate memory for x and y arrays 
     */
    this->targetX = (uint8_t*) malloc(this->noTargets);
    this->targetY = (uint8_t*) malloc(this->noTargets);
    this->targetDir = (char*) malloc(this->noTargets);

    int k = 0, j = 0;
    String cTarget;
    
    /* 
     * Assume with validity checked there are 7 targets. 
     */
    for (; k < 35; k += 5) {

      cTarget = description.substring(k, k + 4);

      this->targetDir[j] = cTarget.charAt(0);
      this->targetX[j] = (cTarget.charAt(1) - '0') * 10 + (cTarget.charAt(2) - '0');
      this->targetY[j] = (cTarget.charAt(3) - '0') * 10 + (cTarget.charAt(4) - '0');

      j++;

    }

    // Determine number of blocked squares
    this->noBlockers = (length - 35) / 5;

    // Allocate memory for x and y arrays
    this->blockerX = (uint8_t*) malloc(this->noBlockers);
    this->blockerY = (uint8_t*) malloc(this->noBlockers);

    k = 0; j = 0;
    for (; k < length; k += 4) {

      cTarget = description.substring(k, k + 4);

      this->blockerX[j] = (cTarget.charAt(1) - '0') * 10 + (cTarget.charAt(2) - '0');
      this->blockerY[j] = (cTarget.charAt(3) - '0') * 10 + (cTarget.charAt(4) - '0');

    }

  }

}

/* 
 * Deconstructor, clear memory 
 */
MazeLayout::~MazeLayout() {

  // Free target allocations
  free(this->targetX);
  free(this->targetY);
  free(this->targetDir);

  // Free blocker allocations
  free(this->blockerX);
  free(this->blockerY);

}

int MazeLayout::getNumberOfTargets() {
  return this->noTargets;
}
  
/*
 * Get the nth target X position
 */
int MazeLayout::getTargetX(int idx) {
  if (idx < this->noTargets)
    return this->targetX[idx];
  else
    return -1;
}

/*
 * Get the nth target Y position
 */
int MazeLayout::getTargetY(int idx) {
  if (idx < this->noTargets)
    return this->targetY[idx];
  else
    return -1;
} 

/* 
 * Is the grid blocked at position (x,y) 
 */
bool MazeLayout::blockedAt(int x, int y) {

  for (int blk = 0; blk < this->noBlockers; blk++) {
    if (this->blockerX[blk] == x && this->blockerY[blk] == y)
      return true;
  }
  return false;

}

// Non-static
bool MazeLayout::isValid() {
  return this->valid;
}

// Static
bool MazeLayout::isValid(String description) {

  if (description.startsWith("*#*#*#*#") && description.endsWith("*#")) {
    
    int length = description.length();
    String stripped = description.substring(8, length - 3);

    // Reset length for stripped text
    length = length - 10;

    // Expecting at least 7 targets
    if (length < 35) {
      return false;
    }

    String cTarget;
    // Check the targets are valid form
    int k;
    for (k = 0; k < 35; k+=5) {
      cTarget = stripped.substring(k, k + 4);
      if (!MazeLayout::isValidTarget(cTarget, false)) return false;
    }

    // Remaining length should be multiple of 5
    if ((length - 28) % 5 != 0)
      return false;

    for (;k < length; k += 5) {
      cTarget = stripped.substring(k, k + 4);
      if (!MazeLayout::isValidTarget(cTarget, true)) return false;
    }

  } else return false;

}

bool MazeLayout::isValidTarget(String task, bool blocker) {

  char c = task.charAt(0);

  // Are we checking a target or a blocking box?
  if (!blocker) {
    // First char to be 'A','B','C','D' or lowercase
    if (c < 'A' || !(c > 'D' && c < 'a') || c > 'd')
      return false;
  }
  else {
    if (c != '*') return false;
  }

  // Next four numbers should be 
  for (int i = 1; i < 5; i++) {
    c = task.charAt(i);
    if (c < '0' || c > '9') return false;
  }

  // Passed all tests
  return true;

}

int MazeLayout::test() {

  Serial.println("Starting");

  if (isValid("*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*1107*0600*1104*0705*1317*#"))
    Serial.println("Valid");

  printf("Completed validation");

  MazeLayout testMaze("*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*1107*0600*1104*0705*1317*#");

  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 10; y++) {
      if (testMaze.blockedAt(x,y)) {
        Serial.print("1");
      } else {
        Serial.print("0");
      }
    }
    Serial.println("");
  }
  return 0;

}
