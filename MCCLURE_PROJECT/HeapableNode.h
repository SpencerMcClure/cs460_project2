//
//  Node.h
//  racetrack_two
//
//  Created by Spencer McClure on 10/28/12.
//
//

#ifndef __racetrack_two__Heapable__Node__
#define __racetrack_two__Heapable__Node__

#include <iostream>
#include <vector>

//this class just has two pointers to two floats, a primary sorting value and a tiebreak
class HeapableNode{
protected:
    float * sortable_value;
    float * tiebreak_value;
        HeapableNode(){}
    HeapableNode(float sort)
    {
        sortable_value = &sort;
    }
    
public:
    float get_sortable_value(){
        return *(sortable_value);
    }
    
    float get_tiebreak_value(){
        return *(tiebreak_value);
    }
    
    void set_sortable_value(float* f)
    {
        sortable_value = f;
    }
    
    void set_tiebreak_value(float* g)
    {
        tiebreak_value = g;
    }
    
};

#endif /* defined(__racetrack_two__HeapableNode__) */
