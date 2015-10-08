// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"
#include "Set.h"

#include <stdio.h>

LocationID findClosestLocationToTarget(LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea);

void decideHunterMove(HunterView gameState) {
	int turnNo = giveMeTheRound(gameState);
	PlayerID IAm = whoAmI(gameState);
	LocationID myPos = whereIs(gameState, IAm);
	LocationID dracPos = whereIs(gameState, PLAYER_DRACULA);

	// THIS PART JUST DEMONSTRATES THE FUNCTION. REMOVE IT IN SUBMISSION.
	if (TRUE) {
		dracPos = LISBON;
		if (validPlace(dracPos)) {
			LocationID decidedLocation = findClosestLocationToTarget(myPos, dracPos, IAm, turnNo, TRUE, TRUE, TRUE);
			printf("DECIDED = %d\n", decidedLocation);
		}
	}

	if (turnNo < 1) {
		registerBestPlay("CD","Let's begin at Castle Dracula");
	} else {
		srand(time(NULL));
		int amountMoves = 0;
		LocationID *moves = whereCanIgo(gameState, &amountMoves, TRUE, FALSE,TRUE);
		int decision = rand() % amountMoves;
		char *choice = idToAbbrev(moves[decision]); 
		registerBestPlay(choice,"I don't think he's showing up...");
		free(moves);
	}
}

LocationID findClosestLocationToTarget(LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea) {
	// Firstly, all the reachable locations are stored.
	Set possiblePlacesSet = reachableLocations(from, player, round, road, rail, sea);

	LocationID decidedLocation = findClosestToTarget(possiblePlacesSet, from, to, player, round, road, rail, sea);
	disposeSet(possiblePlacesSet);
	return decidedLocation;
}