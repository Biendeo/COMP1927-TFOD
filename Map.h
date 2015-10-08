// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Globals.h"
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

Set reachableLocations(LocationID from, PlayerID player, Round round, int road, int rail, int sea);
void fillPlacesOneAway(Set set, LocationID place, TransportID type);
LocationID findClosestToTarget(Set connectedLocationsSet, LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea);

#endif
