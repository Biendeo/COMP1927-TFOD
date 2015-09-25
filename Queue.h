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
void removeElement (Trail trail, LocationID place);
