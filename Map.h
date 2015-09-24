// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Places.h"

typedef struct vNode *VList;

// graph representation is hidden 
typedef struct MapRep *Map;

struct vNode {
	LocationID  v;    // ALICANTE, etc
	TransportID type; // ROAD, RAIL, BOAT
	VList       next; // link to next node
};

struct MapRep {
	int   nV;         // #vertices
	int   nE;         // #edges
	VList connections[NUM_MAP_LOCATIONS]; // array of lists
};

typedef struct edge{
	LocationID  start;
	LocationID  end;
	TransportID type;
} Edge;
 

// operations on graphs 
Map  newMap();  
void disposeMap(Map g); 
void showMap(Map g); 
int  numV(Map g);
int  numE(Map g, TransportID t);

#endif
