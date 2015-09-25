// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Places.h"

typedef unsigned char bool

struct MapRep {
	int   nV;         // #vertices
	int   nE;         // #edges
	bool edges[NUM_MAP_LOCATIONS][NUM_MAP_LOCATIONS][MAX_TRANSPORT+2]  //adjacency matrix of edges containing transport type
	//vertex *locations //could use this to store useful things like maybe vertex degree?
};
// graph representation is hidden 
typedef struct MapRep *Map; 

// operations on graphs 
Map newMap (void);  
void disposeMap (Map m); 
void showMap (Map m); 
int  numV (Map m);
int  numE (Map m, TransportID t);

//in returned array. size of array in sizeArr
//uses BFS to find shortest path from src to dest and stores in order
LocationID *BFS (Map m, LocationID src, TransportID type, int *sizeArr);

//BFS for rail!
void BFSr (Map m, LocationID src, int length, TransportID type,
			LocationID *arr0, LocationID *arr1, LocationID *arr2);
										
//same deal except use DFS. Include both maybe itll be good to use both										
LocationID *DFS (Map m, LocationID src, LocationID dest, int *sizeArr); 

//prototypes for possible future functions
//int hasPathK (Map m, LocationID src, LocationID dest, int k); //uses some matrix multiplication to find if theres a path of length k. NOTE very expensive O(nV^2)

#endif
