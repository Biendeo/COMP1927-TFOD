#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Map.h"
#include "Places.h"
#include "MatrixQueue.h"
#include "Set.h"

static void testConn (void);
static void testReachableLocations (void);
static void testGetClosest (void);

int main (int argc, char *argv[]) {
   printf("Welcome to the Adj Map test suite!\n");

   printf("Now testing Conn...\n");
   testConn();

   printf("Now testing reachableLocations...\n");
   testReachableLocations();

   printf("Now testing getClosest...\n");
   testGetClosest();

   printf("All tests passed! You are awesome!\n");
   return EXIT_SUCCESS;
}

static void testConn (void) {
   Map m = newMap();
   TransportID type[5] = {0};
   assert(numV(m) == 71);
   assert(numE(m) == 198);
   assert(numEdgeType(m, ROAD) == 115);
   assert(numEdgeType(m, RAIL) == 43);
   assert(numEdgeType(m, BOAT) == 40);
   
   assert(connections(m, LONDON, MANCHESTER, type) == 2);
   assert(type[RAIL]);
   assert(type[ROAD]);

   assert(connections(m, LONDON, ENGLISH_CHANNEL, type) == 1);
   assert(type[BOAT]);

   assert(connections(m, LE_HAVRE, ENGLISH_CHANNEL, type) == 1);
   assert(type[BOAT]);

   assert(connections(m, LEIPZIG, HAMBURG, type) == 1);
   assert(type[ROAD]);

   assert(connections(m, PARIS, MARSEILLES, type) == 1);
   assert(type[RAIL]);
  
   assert(connections(m, SANTANDER, NANTES, type) == 1);
   assert(type[BOAT]);

   assert(connections(m, ROME, NAPLES, type) == 3);
   assert(type[RAIL]);
   assert(type[ROAD]);
   assert(type[BOAT]);

   assert(connections(m, MUNICH, BERLIN, type) == 0);
   assert(type[NONE]);

   assert(connections(m, LONDON, CASTLE_DRACULA, type) == 0);
   assert(type[NONE]);
   
   //showMap(m, ROAD);
   disposeMap(m);

   printf("These tests passed.\n");   
}

static void testReachableLocations (void) {
	Map m = newMap();
	Set placesToGo;
	
	placesToGo = reachableLocations(m, PARIS, ROAD);
	assert(getSetSize(placesToGo) == 6);
	assert(isElem(placesToGo, NANTES));
	assert(isElem(placesToGo, LE_HAVRE));
	assert(isElem(placesToGo, CLERMONT_FERRAND));
	assert(isElem(placesToGo, BRUSSELS));
	assert(isElem(placesToGo, GENEVA));
	assert(isElem(placesToGo, STRASBOURG));
	
	placesToGo = reachableLocations(m, LISBON, ROAD);
	assert(getSetSize(placesToGo) == 3);
	assert(isElem(placesToGo, SANTANDER));
	assert(isElem(placesToGo, CADIZ));
	assert(isElem(placesToGo, MADRID));
	assert(!isElem(placesToGo, PARIS));
	
	placesToGo = reachableLocations(m, LISBON, BOAT);
	assert(getSetSize(placesToGo) == 1);
	assert(isElem(placesToGo, ATLANTIC_OCEAN));
	assert(!isElem(placesToGo, GALWAY));
	
	placesToGo = reachableLocations(m, CAGLIARI, BOAT);
	assert(getSetSize(placesToGo) == 2);
	assert(isElem(placesToGo, MEDITERRANEAN_SEA));
	assert(isElem(placesToGo, TYRRHENIAN_SEA));
	
	placesToGo = reachableLocations(m, MARSEILLES, ANY);
	assert(getSetSize(placesToGo) == 8);
	assert(isElem(placesToGo, MEDITERRANEAN_SEA));
	assert(isElem(placesToGo, TOULOUSE));
	assert(isElem(placesToGo, CLERMONT_FERRAND));
	assert(isElem(placesToGo, PARIS));
	assert(isElem(placesToGo, GENEVA));
	assert(isElem(placesToGo, ZURICH));
	assert(isElem(placesToGo, MILAN));
	assert(isElem(placesToGo, GENOA));
	
	disposeMap(m);
	disposeSet(placesToGo);
   printf("These tests passed.\n");  
}

static void testGetClosest (void) {
	Map m = newMap();
	
	assert(getClosestToGoal(m, MADRID, BARCELONA, ROAD, 1) == SARAGOSSA);
	assert(getClosestToGoal(m, MADRID, TOULOUSE, ROAD, 1) == SARAGOSSA);
	assert(getClosestToGoal(m, CAGLIARI, BARCELONA, ROAD, 1) == -1);
	assert(getClosestToGoal(m, ZAGREB, PARIS, ROAD, 1) == MUNICH);
	assert(getClosestToGoal(m, CAGLIARI, NAPLES, BOAT, 1) == TYRRHENIAN_SEA);
	assert(getClosestToGoal(m, MADRID, PARIS, BOAT, 1) == -1);
	assert(getClosestToGoal(m, MADRID, PARIS, ANY, 1) == SARAGOSSA);
	assert(getClosestToGoal(m, MADRID, PARIS, BOAT, 2) == -1);

   assert(getClosestToGoal(m, LISBON, BORDEAUX, RAIL, 2) == SARAGOSSA);
   assert(getClosestToGoal(m, LISBON, PARIS, RAIL, 2) == SARAGOSSA);
   assert(getClosestToGoal(m, LISBON, PARIS, RAIL, 3) == BORDEAUX);
   assert(getClosestToGoal(m, ZAGREB, NUREMBURG, RAIL, 3) == -1);
   assert(getClosestToGoal(m, LISBON, TOULOUSE, RAIL, 2) == -1);
	
   disposeMap(m);
   printf("These tests passed.\n");
}


