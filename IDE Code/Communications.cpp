/*
  Communications.cpp - Library for communicated to the base station / other Arduino
*/

#include "Arduino.h"
#include "Communications.h"

Communications::Communications()
{
}


Communications::~Communications()
{
}

bool Communications::isStart()
{
	return 0;
}

void Communications::getMap(char* oMap)
{
  char map[10] = "abcdf";
  // Need to populate output array, this is better than malloc as that can lead to memory leaks
  for(int i=0; i < 10; ++i){
    oMap[i] = map[i];
  }
}
