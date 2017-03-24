/**
    Point.h
    Purpose: This is a data structure to encapsulate
    the current position of the buggy

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "Point.h"

Point::Point(int8_t x, int8_t y){
  this->x = x;
  this->y = y;
}

void Point::incrementX(){
  this->x++;
}

void Point::decrementX(){
  this->x--;
}

void Point::incrementY(){
  this->y++;
}

void Point::decrementY(){
  this->y--;
}
