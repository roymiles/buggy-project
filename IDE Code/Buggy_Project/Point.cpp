#include "Point.h"

Point::Point(int x, int y){
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
