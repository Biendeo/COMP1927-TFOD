// Set.h
// A set implementation used for TFOD.
// This is specifically for the graph.

#include <stdlib.h>
#include "Places.h"

typedef struct Set *Set;

Set newSet ();
void disposeSet (Set set);
void setAdd (Set set, LocationID place);
void setRemove (Set set, LocationID place);
int isElem (Set set, LocationID place);
LocationID *copySetToArray (Set set);
Set copyArrayToSet (LocationID *arr, int size);