all: racetrack
racetrack: racetrack.o breakthemovementvector.o heapablenode.o heap.o point.o state.o
	g++ Racetrack.o BreakTheMovementVector.o HeapableNode.o Heap.o Point.o State.o
point.o: Point.cpp
	g++ -c Point.cpp
heapablenode.o: HeapableNode.cpp
	g++ -c HeapableNode.cpp
heap.o: Heap.cpp
	g++ -c Heap.cpp
state.o: State.cpp
	g++ -c State.cpp
breakthemovementvector.o: BreakTheMovementVector.cpp
	g++ -c BreakTheMovementVector.cpp
racetrack.o: Racetrack.cpp
	g++ -c Racetrack.cpp
clean:
	rm -rf *o racetrack
		