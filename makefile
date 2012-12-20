all: racetrack

racetrack: racetrack.o breakthemovementvector.o heapablenode.o heap.o point.o state.o
	g++ racetrack.o breakthemovementvector.o heapablenode.o heap.o point.o state.o

point.o: point.cpp
	g++ -c point.cpp

heapablenode.o: heapablenode.cpp
	g++ -c heapablenode.cpp

heap.o: heap.cpp
	g++ -c heap.cpp

state.o: state.cpp
	g++ -c state.cpp

breakthemovementvector.o: breakthemovementvector.cpp
	g++ -c breakthemovementvector.cpp

racetrack.o: racetrack.cpp
	g++ -c racetrack.cpp

clean:
	rm -rf *o racetrack