// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Places.h"

#include "Set.h"

typedef struct vNode *VList;

// graph representation is hidden 
typedef struct MapRep *Map;

// operations on graphs 
Map  newMap();  
void disposeMap(Map g); 
void showMap(Map g); 
int  numV(Map g);
int  numE(Map g, TransportID t);

void fillPlacesOneAway(Set set, LocationID place, TransportID type);

#endif
