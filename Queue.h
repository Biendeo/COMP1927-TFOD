// Queue.h
// A queue implementation used for TFOD.
// This is specifically for the trail.

#include <stdlib.h>
#include "Places.h"


//THIS can be for the actual trail
typedef struct Trail *Trail;

Trail newTrail (int size);
void disposeTrail (Trail trail);
void prepend (Trail trail, LocationID place);
LocationID showElement (Trail trail, int index);


//this is for graph traversal purposes, feel free to use
typedef struct QueueRep *Queue;

Queue newQueue(void); //builds new empty queue
void disposeQueue(Queue); //gets rid of a queue
void enterQueue(Queue,LocationID); //push to the queue
LocationID leaveQueue(Queue); //pop the queue
int  emptyQueue(Queue); //see if queue is empty
void showQueue(Queue q); //reveal whats in the queue