/**
    Point.h
    Purpose: This is a data structure to encapsulate
    the current position of the buggy

    @author Roy Miles
    @version 1.0 11/12/2016
*/


#ifndef Point_h
#define Point_h

#include "Arduino.h"

#pragma once

class Point
{
public:
  int8_t x;
  int8_t y;

  Point(int8_t x, int8_t y);
  ~Point();

  void incrementX();
  void decrementX();
  void incrementY();
  void decrementY();

};

#endif

