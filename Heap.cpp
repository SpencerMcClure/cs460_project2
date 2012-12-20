//
//  Heap.cpp
//  racetrack_two
//
//  Created by Spencer McClure on 10/28/12.
//  A MIN HEAP
//

#include "Heap.h"

//default constructor
//takes the higher g in tiebreak
Heap::Heap()
{
    
    nodes_added=0;
    nodes = std::vector<HeapableNode *>(1);
    gHigh=true;
}

//heap with option of tiebreak with g working one of two ways
Heap::Heap(bool gHi)
{
    nodes_added = 0;
    nodes = std::vector<HeapableNode *>(1);
    gHigh = gHi;
}

Heap::~Heap(){
    
}

//adds a node to the bottom and begins reheaping up
void Heap::addNode(HeapableNode * n)
{
    if(nodes.size()==1||nodes_added==nodes.size()-1)
        nodes.push_back(n);
    else
        nodes[nodes_added+1] = n;
    nodes_added++;
    reheapUp(nodes_added);
    
}

//looks at value of i and compares to the parent
//switches if need parent is greater than value at i
//reheaps p again if switched
//depending on gHigh takes either the higher or lower g in tiebreak
void Heap::reheapUp(int i)
{
    
    if((i>1)&&nodes[getParent(i)]->get_sortable_value() > nodes[i]->get_sortable_value())
    {
        HeapableNode *temp = nodes[getParent(i)];
        nodes[getParent(i)] = nodes[i];
        nodes[i] = temp;
        reheapUp(getParent(i));
    }
    else if(gHigh&&(i>1)&&nodes[getParent(i)]->get_sortable_value() == nodes[i]->get_sortable_value() && nodes[getParent(i)]->get_tiebreak_value() < nodes[i]->get_tiebreak_value())
    {
        HeapableNode *temp = nodes[getParent(i)];
        nodes[getParent(i)] = nodes[i];
        nodes[i] = temp;
        reheapUp(getParent(i));
    }
    else if(!gHigh&&(i>1)&&nodes[getParent(i)]->get_sortable_value() == nodes[i]->get_sortable_value() && nodes[getParent(i)]->get_tiebreak_value() > nodes[i]->get_tiebreak_value())
    {
        HeapableNode *temp = nodes[getParent(i)];
        nodes[getParent(i)] = nodes[i];
        nodes[i] = temp;
        reheapUp(getParent(i));
    }
}


//looks at value of i and compares to the two children
//looks at the smallest child
//if smaller than the smallest child, switches, comparing first by j
//then by g if a tiebreak is required
void Heap::reheapDown(int i)
{
    
   
    
    if(getLeft(i)<=nodes_added&&getRight(i)<=nodes_added)
    {
        if(nodes[getLeft(i)]->get_sortable_value()>nodes[getRight(i)]->get_sortable_value()) //right is less than left
        {
            if(nodes[getRight(i)]->get_sortable_value() < nodes[i]->get_sortable_value()) //right is less than top
            {
                HeapableNode *temp = nodes[getRight(i)];
                nodes[getRight(i)] = nodes[i];
                nodes[i] = temp;
                reheapDown(getRight(i));
            }
            else if(gHigh&&nodes[getRight(i)]->get_sortable_value() == nodes[i]->get_sortable_value() && nodes[getRight(i)]->get_tiebreak_value() > nodes[i]->get_tiebreak_value())
            {
                HeapableNode *temp = nodes[getRight(i)];
                nodes[getRight(i)] = nodes[i];
                nodes[i] = temp;
                reheapDown(getRight(i));
            }
            else if(!gHigh&&nodes[getRight(i)]->get_sortable_value() == nodes[i]->get_sortable_value() && nodes[getRight(i)]->get_tiebreak_value() < nodes[i]->get_tiebreak_value())
            {
                HeapableNode *temp = nodes[getRight(i)];
                nodes[getRight(i)] = nodes[i];
                nodes[i] = temp;
                reheapDown(getRight(i));
            }
        }
        else
        {
            if(nodes[getLeft(i)]->get_sortable_value() < nodes[i]->get_sortable_value()) //left is less than top (&& right is > left)
            {
                HeapableNode *temp = nodes[getLeft(i)];
                nodes[getLeft(i)] = nodes[i];
                nodes[i] = temp;
                reheapDown(getLeft(i));
            }
            else if(gHigh&&nodes[getLeft(i)]->get_sortable_value() == nodes[i]->get_sortable_value() && nodes[getLeft(i)]->get_tiebreak_value() > nodes[i]->get_tiebreak_value()) //left is less than top (&& right is > left)
            {
                HeapableNode *temp = nodes[getLeft(i)];
                nodes[getLeft(i)] = nodes[i];
                nodes[i] = temp;
                reheapDown(getLeft(i));
            }
            else if(!gHigh&&nodes[getLeft(i)]->get_sortable_value() == nodes[i]->get_sortable_value() && nodes[getLeft(i)]->get_tiebreak_value() < nodes[i]->get_tiebreak_value()) //left is less than top (&& right is > left)
            {
                HeapableNode *temp = nodes[getLeft(i)];
                nodes[getLeft(i)] = nodes[i];
                nodes[i] = temp;
                reheapDown(getLeft(i));
            }

        }
    }
}

//math functions to access children and parents of nodes
int Heap::getLeft(int i)
{
    
    return i*2;
}

int Heap::getRight(int i)
{
    return i*2 +1;
}

int Heap::getParent(int i)
{
    return i / 2;
}

//returns pointer to the top
HeapableNode * Heap::getTop()
{
    return nodes[1];
}

//removes the top, by swapping with the bottom node, deleting it and reheaping down
void Heap::removeTop()
{
    nodes[1] = nodes[nodes_added];
    nodes[nodes_added] = NULL;
    nodes_added--;
    reheapDown(1);
}


