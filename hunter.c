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

typedef enum HUNTER_MESSAGE {
	WAITING_FOR_THE_UNKNOWN,
	HEALING,
	ON_THE_TRAIL,
	SITTING_TIGHT,
	CAMPING_THE_CASTLE
} HunterMessage;

LocationID findClosestLocationToTarget(LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea);
LocationID getGoal(HunterView gameState);
char *givePresetMessage(HunterView gameState, HunterMessage goal);

void decideHunterMove(HunterView gameState) {
	int turnNo = giveMeTheRound(gameState);
	PlayerID IAm = whoAmI(gameState);
	LocationID myPos = whereIs(gameState, IAm);
	LocationID dracPos = whereIs(gameState, PLAYER_DRACULA);
	LocationID decidedLocation;
	char *place;
	char *message;

	// START DETERMINING A GOAL HERE

	switch (IAm) {
		case PLAYER_LORD_GODALMING:
		default:
			if (dracPos == CITY_UNKNOWN || dracPos == SEA_UNKNOWN && turnNo > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			}
			break;
		case PLAYER_DR_SEWARD:
			if (dracPos == CITY_UNKNOWN || dracPos == SEA_UNKNOWN && turnNo > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			}
			break;
		case PLAYER_VAN_HELSING:
			if (dracPos == CITY_UNKNOWN || dracPos == SEA_UNKNOWN && turnNo > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			}
			break;
		case PLAYER_MINA_HARKER:
			if (dracPos == CITY_UNKNOWN || dracPos == SEA_UNKNOWN && turnNo > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			} else {

			}
			break;
	}

	// STOP DETERMINING A GOAL HERE

	place = idToAbbrev(decidedLocation);
	registerBestPlay(place, message);
}

LocationID findClosestLocationToTarget(LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea) {
	// Firstly, all the reachable locations are stored.
	Set possiblePlacesSet = reachableLocations(from, player, round, road, rail, sea);

	LocationID decidedLocation = findClosestToTarget(possiblePlacesSet, from, to, player, round, road, rail, sea);
	disposeSet(possiblePlacesSet);
	return decidedLocation;
}

// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
char *givePresetMessage(HunterView gameState, HunterMessage code) {
	switch (code) {
		case WAITING_FOR_THE_UNKNOWN:
			return "Systematic research time!";
		case HEALING:
			return "Heal it up.";
		case ON_THE_TRAIL:
			return "";
		case SITTING_TIGHT:
		default:
			return "";
		case CAMPING_THE_CASTLE:
			return "";
	}
}
