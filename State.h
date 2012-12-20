//
//  State.h
//  racetrack_two
//
//  Created by Spencer McClure on 10/29/12.
//
//

#ifndef __racetrack_two__State__
#define __racetrack_two__State__

#include <iostream>
#include "HeapableNode.h"
#include "Point.h"
#include <math.h>

//This class represents a searchable state
//It keeps track of f, g and h values, previous states, 
//  previous speeds, as well as information such as valid (ie on road or off)
//  and whether its a finish (goal) state

//It is of superclass HeapableNode so that the States can be easily Heaped
class State:public HeapableNode{


public:
//heuristic/path values
    float g, h,f;
    
//where it is in the map
    Point* loc;
    
    //whether it has been evaluated yet
    bool eval;
    //whether it is on road or off
    bool valid;
    //whether it is finish state or not
    bool finish;
    
    int speed;
    
    //prior state, should this state be a part of the solution
    State * prev;
    
    //prior speed
    Point * prev_speed;
    
    //constructs using the heapablenode constructor so the heap knows which values to compare
    State():HeapableNode(f){f=-1;h=0; g=-1; valid=true; finish=false; eval=false; this->set_sortable_value(&f); this->set_tiebreak_value(&g);}
        
    State(Point* location):HeapableNode(f){f=-1; h=0; g=-1; eval=false; valid=true; loc = location; this->set_sortable_value(&f); this->set_tiebreak_value(&g); finish=false; eval=false;}
    
    
    //sets g and updates f
    void setG(float newg){
        g = newg;
        f = h+g;
        eval=true;
    }
    
    //assigns the prev pointer to the state prior
    void setPrev(State* prev_state)
    {
        prev = prev_state;
    }
    
    //setter
    void setPrevSpeed(Point* prev_spd);
    
    //if finish, then h=0, else assigns h to the appropriate heuristic function
    void setH(int heuristic, Point *averageFinish)
    {
        if(finish){
            this->h = 0;
            return;
        }
        switch(heuristic)
        {
            case(0):
                this->h = this->calcEuclideanDistance(averageFinish) / 42;
                return;
                
            case(1):
                this->h = 0.8*this->calcEuclidDistanceWithSpeed(averageFinish);
                return;
        }
        
    
    }
    
    //calculates the euclidean distance from this state to the average finish state
    float calcEuclideanDistance(Point* f)
    {
        return sqrt((f->x - this->loc->x)*(f->x - this->loc->x) + (f->y - this->loc->y)*(f->y - this->loc->y));
    }
    
    //calculates the euclidean distance from this state to the average finish state
    float calcEuclideanDistance(Point* f, Point*g)
    {
    return sqrt((f->x - g->x)*(f->x - g->x) + (f->y - g->y)*(f->y - g->y));
    }
    
     //calculates the manhattan distance from this state to the average finish state
    float calcManhattanDistance(Point* f)
    {
        return abs(f->x - this->loc->x) + abs(f->y - this->loc->y);
    
    }
    
    //See answer to #1 for explanation
    float calcEuclidDistanceWithSpeed(Point* f)
    {
        Point* targetSpeed;
        int xdist = f->x - this->loc->x;
        int ydist = f->y - this->loc->y;
        float euclid_dist = sqrt((f->x - this->loc->x)*(f->x - this->loc->x) + (f->y - this->loc->y)*(f->y - this->loc->y));
          int counter = 0;
    Point* currentSpeed = new Point(this->prev_speed->x, this->prev_speed->y);
    Point* currentPoint = new Point(this->loc->x, this->loc->y);
        Point *speed_after_accel;
        while(euclid_dist>84)
        {
            float normalized_x = xdist / euclid_dist;
            float normalized_y = ydist / euclid_dist;
            targetSpeed = new Point(normalized_x*30, normalized_y*30);
            if(targetSpeed->x==currentSpeed->x && targetSpeed->y==currentSpeed->y)
                break;
                
            for(int i = 0; i < euclid_dist/42; i++)            {
            
                currentPoint->x+=currentSpeed->x;
                currentPoint->y+=currentSpeed->y;
                if(currentSpeed->x < targetSpeed->x)
                    currentSpeed->x++;
                else if(currentSpeed->x > targetSpeed->x)
                    currentSpeed->x--;
            
        
                if(currentSpeed->y < targetSpeed->y)
                    currentSpeed->y++;
                else if(currentSpeed->y > targetSpeed->y)
                    currentSpeed->y--;
                
                

                counter++;
            }
                xdist = f->x - currentPoint->x;
                ydist = f->y - currentPoint->y;
                speed_after_accel  = currentSpeed;
                euclid_dist = calcEuclideanDistance(f, currentPoint);
        }
        int steps_remaining = euclid_dist / sqrt(currentSpeed->x*currentSpeed->x + currentSpeed->y*currentSpeed->y);
        counter+=steps_remaining;

       
        return counter;
    }
};


#endif /* defined(__racetrack_two__State__) */
