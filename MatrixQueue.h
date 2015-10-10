// MatrixQueue.h ... interface to simple Queue of ints
// Adapted from John Shepherd, September 2015

#ifndef MATRIXQUEUE_H
#define MATRIXQUEUE_H
#include "Places.h"

typedef struct QueueRep *Queue;

// Function signatures
Queue newQueue(void);
void disposeQueue(Queue);
void enterQueue(Queue, LocationID place);
LocationID leaveQueue(Queue);
int emptyQueue(Queue);
void showQueue(Queue q);

#endif
