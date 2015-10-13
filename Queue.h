// Queue.h
// A queue implementation used for TFOD.
// This is specifically for the trail.

#include <stdlib.h>
#include "Places.h"

// Trail
typedef struct Trail *Trail;

Trail newTrail(int size);
void disposeTrail(Trail trail);
void prepend(Trail trail, LocationID place);
LocationID showElement(Trail trail, int index);
void removeElement(Trail trail, LocationID place);

// Queue
typedef struct Queue *Queue;

Queue newQueue();
void disposeQueue(Queue queue);
void queueAdd(Queue queue, LocationID value);
LocationID queuePop(Queue queue);
int getQueueSize(Queue queue);