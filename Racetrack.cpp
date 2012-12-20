//
//  Racetrack.cpp
//  Racetrack
//
//  Created by Spencer McClure on 10/24/12.
//
//

#include "Racetrack.h"
#include <iostream>
#include <fstream>

#define MAX_SPEED 30
#define EUC 0 //0= euclidean, 1=manhattan
#define MANHATTAN 1

//Constructor
//Initializes starting car_velocity to 0,0.
Racetrack::Racetrack()
{
    car_vel = Point(0,0);
    nodesExpanded=0;
    h0=1;
    to_be_expanded = new Heap(true);
}

//Constructor
Racetrack::Racetrack(bool h, bool gCompare)
{
    h0 = h;
    gComp = gCompare;
    to_be_expanded = new Heap(gComp);
    nodesExpanded=0;
}

//takes a char* filename to read in
int Racetrack::readMap(const char * filename)
{
    string inp;
    ifstream mapfile(filename);
    finish_states = new vector<State*>();
    if(mapfile.is_open())
    {
        //records width, else cancels function
        getline(mapfile,inp);
        if(inp.find("WIDTH")==0){
            inp = inp.substr(5,inp.length());
            this->width = atoi(inp.c_str());
        }
        else
            return 0;
        //records height, else cancels function
        getline(mapfile,inp);
        if(inp.find("HEIGHT")==0){
            inp = inp.substr(6,inp.length());
            this->height = atoi(inp.c_str());
            
        }
        else
            return 0;
        
        
        //map of characters, used for debugging purposes mostly
        this->map = new vector <vector<char> *>();
        
        //3d vector of states - each 3rd-dimension vector is a list of explored states for a specific location
        //Consists of a lot of length-one vectors for off-road states
        this->statemap = new vector< vector< vector<State*> *> *>();
        for(int i = 0; i < height; i++)
        {
            //the 2nd dimension vector for the char map
            vector<char> *linevec = new vector<char>();
            
            //the 2nd dimension vector for the state map
            vector< vector<State*> *> *statevec = new vector< vector<State*> *>();
            
            //reads a line of input
            getline(mapfile,inp);
            const char * line = inp.c_str();
            
            
            for(int j=0; j < inp.length(); j++)
            {
                //the '3rd-dimension vector' will be added into the 2nd dimension vector for the statemap
                vector<State *> *stateheap = new vector<State *>();
                
                
                State * tempState = new State(new Point(j,i));
                if(line[j]=='S')
                {
                    
                    //to allow for better backtracking, creates the initial state and the second state right away
                    car_loc = Point(j,i);
                    
                    //first state
                    State* startState = new State(new Point(j,i));
                    startState->setPrev(NULL);
                    startState->setPrevSpeed(new Point(0,0));
                    startState->setG(0);
                    
                    //second state
                    tempState->setG(1);
                    tempState->setPrev(startState);
                    tempState->setPrevSpeed(new Point(0,0));
                    tempState->valid = true;
                    tempState->eval=true;
                    
                    //second state is first and only state added to heap
                    to_be_expanded->addNode(tempState);
                }
            
                // tempState is an off-road cell, and thus invalid
                else if(line[j]=='#')
                {
                    
                    tempState->valid=false;
                }
            
                //tempState is a finish cell
                else if(line[j]=='F')
                {
                
                    
                    tempState->finish=true;
                    finish_states->push_back(tempState);
                }
                
                //tempState is a normal undiscoverd cell
                else if(line[j]==' ')
                {
                    tempState->setG(0);
                    tempState->setPrev(NULL);
                    tempState->setPrevSpeed(0);
                    tempState->valid = true;
                    tempState->eval=false;

                }
            
            
                //pushes back all vectors appropriately for this location
                linevec->push_back(line[j]);
                stateheap->push_back(tempState);
                statevec->push_back(stateheap);
            }
            //pushes back all vectors appropriately for the whole line of txt
            map->push_back(linevec);
            statemap->push_back(statevec);
            
        }
        //calculate average finish location for heuristic-use
        averageFinishLoc = calcAverageFinishLoc();
        return 1;
        
    }
    //if file didn't open
    return 0;
}

//sums all x and y respectively, and returns a point to represent the average
Point* Racetrack::calcAverageFinishLoc()
{
    int sumx=0;
    int sumy=0;
    for(int i = 0; i < finish_states->size(); i++)
    {
        sumx+=finish_states->at(i)->loc->x;
        sumy+=finish_states->at(i)->loc->y;
    }
    return new Point(sumx / finish_states->size(), sumy / finish_states->size());
}

//first determines the bounds of coordinates that should be checked for valid places to move
//then creates a new state for the location and speed and adds the new state to the heap of states
void Racetrack::expandNode(State *loc_ex)
{
    
    //creating the bounds
    int lower_vel_x = loc_ex->prev_speed->x>-MAX_SPEED ? loc_ex->prev_speed->x-1 : -MAX_SPEED;
    int higher_vel_x = loc_ex->prev_speed->x<MAX_SPEED ? loc_ex->prev_speed->x+1 : MAX_SPEED;
    int lower_vel_y = loc_ex->prev_speed->y>-MAX_SPEED ? loc_ex->prev_speed->y-1 : -MAX_SPEED;
    int higher_vel_y = loc_ex->prev_speed->y<MAX_SPEED ? loc_ex->prev_speed->y+1 : MAX_SPEED;
    
    //looping through the various possible destinations
    for(int i = lower_vel_x; i <= higher_vel_x; i++)
    {
        for(int j = lower_vel_y; j <= higher_vel_y; j++)
        {
        
            //using breakTheMovementVector to get a list of moves
            vector<int*> moves = breakTheMovementVector(i, j);
            Point *temploc = new Point(loc_ex->loc->x, loc_ex->loc->y);
            bool valid_move =true;
            
            //looping through the moves
            for(int k = 0; k < moves.size(); k++)
            {
                //if not valid move, break;
                if(!(statemap->at(temploc->y+moves[k][1])->at(temploc->x+moves[k][0])->at(0))->valid)
                {
                    valid_move = false;
                    break;
                }
                //else advance to the next location
                temploc->y+=moves[k][1];
                temploc->x+=moves[k][0];
            }
        
            if(!valid_move)
                continue;
                
            //check if an activated state exists at that location yet
            vector<State*> *possibleStates = statemap->at(loc_ex->loc->y+j)->at(loc_ex->loc->x+i);
            bool noOtherState = false;
            
            //if an activated state doesnot exist, it sets the state information there appropriately
            //and adds it to the heap, to_be_expanded
            if(! possibleStates->at(0)->eval)
            {
                possibleStates->at(0)->prev = loc_ex;
                possibleStates->at(0)->prev_speed = new Point(i,j);
                possibleStates->at(0)->setH(h0, averageFinishLoc);
                possibleStates->at(0)->setG(loc_ex->g+1);
                possibleStates->at(0)->eval=true;
                to_be_expanded->addNode(possibleStates->at(0));

            }
            
            //else there are several possible states that may be duplicates
            else{
                noOtherState=true;
                //loops through possible states, if there is a duplicate, noOtherState=false;
                for(int k=0; k < possibleStates->size(); k++)
                {
                    if(possibleStates->at(k)->prev!=NULL)
                    {
                        if(possibleStates->at(k)->prev->loc->x == loc_ex->loc->x && possibleStates->at(k)->prev->loc->y == loc_ex->loc->y)
                        {
                            noOtherState=false;
                        }
                    }
                }
            }
            
            
          
            //if the state doesn't already exist, creates the new state, adds it to heap.
            if(noOtherState)
            {
                State * newstate = new State(statemap->at(loc_ex->loc->y+j)->at(loc_ex->loc->x+i)->at(0)->loc);
                newstate->prev = loc_ex;
                newstate->prev_speed = new Point(i,j);
                
                //calculates h before g
                newstate->setH(h0, averageFinishLoc);
                
                //increments g, also updates j
                newstate->setG(loc_ex->g+1);
                
                //adds it to statemap
                statemap->at(loc_ex->loc->y+j)->at(loc_ex->loc->x+i)->push_back(newstate);
                
                //adds it to heap
                to_be_expanded->addNode(newstate);
                
            }
            
        }
    }
}


//begins the loop, expanding the top of the heap until it the top is a finish
//node or there are no more states to expand
void Racetrack::startSearch()
{
    bool found = false;
    while(to_be_expanded->getTop()!=NULL)
    {
        
        //expands, then removes.
        expandNode((State*)to_be_expanded->getTop());
        //checks if the expanded state was a finish location
        State* top = (State*)to_be_expanded->getTop();
        to_be_expanded->removeTop();
        nodesExpanded++;
        
        
        if(statemap->at(top->loc->y)->at(top->loc->x)->at(0)->finish)
        {
            found=true;
            startBacktrack(top);
            break;
        }
    }
    if(!found)
        cout<<"No solution found"<<endl;
}


//backtracks through the states after a finish state is found
//also handles the formatting of text, by storing the speed of the node before
void Racetrack::startBacktrack(State* f)
{
    //list of strings to be reversed -- trying to avoid many prepends to a single string
    vector<string *> *steps = new vector<string *>();
    
    string *s = new string();
    
    
    State* state = f;
    
    //temporary stringstream to store each line (step) of text
    stringstream *temp = new stringstream(std::stringstream::in | std::stringstream::out);
    
    //final state, added first
    (*temp)<<"Step: "<<state->g<<" {P:("<<state->loc->x<<","<<state->loc->y<<")}";
    *s = temp->str();
    steps->push_back(s);
    
    //stored to be displayed along with the prev state
    Point * speed_backup = f->prev_speed;
    
    //for calculating the average
    float speedsum =0;
    
    //loops through the previous states until the first state (the only state without a prev)
    while(state->prev!=NULL)
    {
        state = state->prev;
        stringstream *temp = new stringstream(std::stringstream::in | std::stringstream::out);
        string *s = new string();
        (*temp)<<"Step: "<<state->g<<" {P:("<<state->loc->x<<","<<state->loc->y<<") M:("<<speed_backup->x<<","<<speed_backup->y<<") g="<<state->g<<" h="<<state->h<<" f="<<state->f<<" Speed="<<speedFromPoint(speed_backup)<<" }";
        *s = temp->str();
        speedsum+=speedFromPoint(speed_backup);
        steps->push_back(s);
        speed_backup = state->prev_speed;
    }
    ofstream outfile(this->outputname);
    
    //reverse outputs the vector list of strings
    for(unsigned long i = steps->size(); i > 0; i--)
    {
        cout<<*(steps->at(i-1))<<endl;
        outfile<<*(steps->at(i-1))<<endl;
    }
    
    //outputs the other data collected
    cout<<"Average Speed: "<<speedsum / (float)steps->size()-1<<endl;
    cout<<"Number of steps: "<<steps->size()-1<<endl;
    cout<<"Number of expanded nodes: "<<nodesExpanded<<endl;
    outfile<<"Average Speed: "<<speedsum / (float)steps->size()-1<<endl;
    outfile<<"Number of steps: "<<steps->size()-1<<endl;
    outfile<<"Number of expanded nodes: "<<nodesExpanded<<endl;
    gettimeofday(&tv2, NULL);
    float runtime=1.0*(tv2.tv_sec - tv1.tv_sec) + 1.0*(tv2.tv_usec - tv1.tv_usec)/1000000.0;
    cout<<"Runtime = "<<runtime<<" sec"<<endl;
    outfile<<"Runtime = "<<runtime<<" sec"<<endl;
}

//converts a point into a speed by comparing the absolute value of
// x and y
int Racetrack::speedFromPoint(Point* p)
{
    int abx = abs(p->x);
    int aby = abs(p->y);
    if(abx>=aby)
        return abx;
    return aby;
}



//the main function, optionally reads in the command-line or performs hard-coded run for debugging
int main(int argv, char ** argc)
{
    Racetrack r;
    string strng;
    //cout<<"argv: "<<argv<<endl;
    if(argv==7)
    {
        //cout<<"argc[1]="<<argc[1]<<" (equals? "<<strcmp(argc[1],"-i")<<")"<<endl;
        if(strcmp(argc[1], "-i")==0 && strcmp(argc[3], "-o")==0 && (strcmp(argc[5], "-h1")==0||strcmp(argc[5], "-h2")==0) && (strcmp(argc[6], "-t1")==0||strcmp(argc[6], "-t2")==0))
        {
            //cout<<"Read command-line"<<endl;
            bool h0 = (strcmp(argc[5],"-h1")) ? true:false;
            bool t1 = (strcmp(argc[6],"-t1")) ? true:false;
            r= Racetrack(h0, t1);
            strng = argc[2];
            strng.c_str();
            r.outputname = argc[4];
            
        }
            
    }
    else
    {
        r = Racetrack(true,false);
        strng = "/Users/Spencer/Documents/college/ai/test/racetrack_two/racetrack/racetrack3.txt";
        
    }
    //outputs a 1 if map reads correctly, else a 0
    cout<<r.readMap(strng.c_str())<<endl;
    
    //time code to record time between when search begins and ends
    gettimeofday(&tv1, NULL);
    r.startSearch();
    
    
  
}