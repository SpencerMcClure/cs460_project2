//
//  Heap.h
//  racetrack_two
//
//  Created by Spencer McClure on 10/28/12.
//
//

#ifndef __racetrack_two__Heap__
#define __racetrack_two__Heap__

#include <iostream>
#include "HeapableNode.h"

//This min-heap stores heapablenode pointers that provide two values to compare, a primary and a tiebreak
class Heap{
    std::vector<HeapableNode *> nodes;
    int nodes_added;
    bool gHigh;
public:
    void addNode(HeapableNode * n);
    int getLeft(int i);
    int getRight(int i);
    int getParent(int i);
    void reheapUp(int i);
    void reheapDown(int i);
    void removeTop();
    HeapableNode * getTop();
    Heap();
    Heap(bool gHigh);
    ~Heap();
    
    
};

#endif /* defined(__racetrack_two__Heap__) */
