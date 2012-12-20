//
//  Racetrack.h
//  Racetrack
//
//  Created by Spencer McClure on 10/24/12.
//
//

#ifndef __Racetrack__Racetrack_
#define __Racetrack__Racetrack__

#include <iostream>
#include <vector>
#include <sstream>
#include "BreakTheMovementVector.h"
#include "Heap.h"
#include "Point.h"
#include "State.h"
#include <sys/time.h>

    #define TOPSPEED 30;
struct timeval tv1, tv2;

using namespace std;

class Racetrack {

public:
    
    

    float runtime;
    

        
    Heap *to_be_expanded;
    vector< vector<char> *> *map;
    vector< vector< vector<State*> *> *> *statemap;
    vector< State*> *finish_states;
    Point car_loc;
    Point car_vel;
    Point *averageFinishLoc;
    int height, width, nodesExpanded;
    bool gComp, h0;
    char* outputname;
    
    Racetrack();
    Racetrack(bool gComp, bool h0);
    int readMap(const char * filename);
    void expandNode(State *loc_ex);
    void startBacktrack(State* top);
    int speedFromPoint(Point*);
    void startSearch();
    Point* calcAverageFinishLoc();
};


#endif /* defined(__Racetrack__Racetrack__) */
