// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H
#include "Places.h"
#include "Set.h"

// graph representation is hidden 
typedef struct MapRep *Map; 

// operations on graphs 
Map newMap (void);  
void disposeMap (Map m); 
void showMap (Map m, TransportID type); 
int  numV (Map m);
int  numE (Map m);	//total number of edges
int  numEdgeType (Map m, TransportID t); //edges of particular type

//given src and dest, return number of direct connections, and type[] has types of transport
int connections (Map m, LocationID src, LocationID dest, TransportID type[]);

//returns a set of places reachable immediately from src
Set reachableLocations (Map m, LocationID src, TransportID type);

//uses BFS to find closest place to dest from src
//only use rail if src and dest are linked by railways
LocationID getClosestToGoal (Map m, LocationID src, LocationID dest,
							TransportID type, int length);
										
#endif
