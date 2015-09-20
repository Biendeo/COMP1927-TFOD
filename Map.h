// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Places.h"

typedef struct vNode *vertex;

struct vNode {
	LocationID  v;    // ALICANTE, etc
	int vDegree		//tells how many edges incident with this vertex, for use in euler pathing/tours 
};

typedef struct edgeType{
	TransportID  road;
	TransportID  rail;
	TransportID boat;
} EdgeType;

struct MapRep {
	int   nV;         // #vertices
	int   nE;         // #edges
	EdgeType **edges  //adjacency matrix of edges
	vertex *locations //use this to store useful things like maybe vertex degree?
};

// graph representation is hidden 
typedef struct MapRep *Map; 

// operations on graphs 
Map newMap (void);  
void disposeMap (Map g); 
void showMap (Map g); 
int  numV (Map g);
int  numE (Map g, TransportID t);
LocationID *BFS (Map m, LocationID src, LocationID dest, int *sizeArr);	//uses BFS to find shortest path from src to dest and stores in order
																			//in returned array. size of array in sizeArr
LocationID *DFS (Map m, LocationID src, LocationID dest, int *sizeArr); //same deal except use DFS. Include both maybe itll be good to use both
//prototypes for possible future functions
//int hasPathK (Map m, LocationID src, LocationID dest, int k); //uses some matrix multiplication to find if theres a path of length k. NOTE very expensive O(nV^2)

#endif
