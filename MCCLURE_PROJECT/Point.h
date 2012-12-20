//
//  Point.h
//  racetrack_two
//
//  Created by Spencer McClure on 10/29/12.
//
//

#ifndef __racetrack_two__Point__
#define __racetrack_two__Point__

#include <iostream>
#include <sstream>
#include <string>

//A class to store a pair of related terms, labeled as x and y
class Point{
public:
    int x;
    int y;
    Point(){}
    Point(int newx, int newy){x=newx;y=newy;}
    const std::string toString(){ std::stringstream s(std::stringstream::in | std::stringstream::out); s << "(" << this->x << ", "<< this->y <<")"; return s.str(); }
};

#endif /* defined(__racetrack_two__Point__) */
