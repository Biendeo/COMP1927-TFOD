// Queue.h
// A queue implementation used for TFOD.
// This is specifically for the trail.

#include <stdlib.h>
#include "Places.h"

#define NULL_TERM -1

typedef struct Trail *Trail;

Trail newTrail (int size);
void disposeTrail (Trail trail);
void prepend (Trail trail, LocationID place);
LocationID showElement (Trail trail, int index);