#ifndef Point_h
#define Point_h

#pragma once

class Point
{
public:
  int x;
  int y;

  Point(int x, int y);
  ~Point();

  void incrementX();
  void decrementX();
  void incrementY();
  void decrementY();

};

#endif

