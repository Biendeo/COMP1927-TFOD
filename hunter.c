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

//given source, destination, player, round numer, and travel method, will find closest location to target
static LocationID findClosestLocationToTarget(LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea);
//tells where dracula was last seen
static LocationID whereWasDraculaLastSeen(HunterView gameState);
static char *givePresetMessage(HunterView gameState, HunterMessage goal);

static LocationID godalmTurn(HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round); 
static LocationID sewardTurn(HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round);
static LocationID helsingTurn(HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round);
static LocationID minaTurn(HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round);

void decideHunterMove(HunterView gameState) {
	Round round = giveMeTheRound(gameState);
	PlayerID IAm = whoAmI(gameState);
	LocationID myPos = whereIs(gameState, IAm);
	LocationID dracPos = whereWasDraculaLastSeen(gameState);
	LocationID decidedLocation;
	LocationID nextLocation;
	char *place;
	char *message;

	// START DETERMINING A GOAL HERE
	switch (IAm) {
		case PLAYER_LORD_GODALMING:
		default:
			decidedLocation = godalmTurn(gameState, message, dracPos, myPos, round);
			break;
		case PLAYER_DR_SEWARD:
			decidedLocation = sewardTurn(gameState, message, dracPos, myPos, round);
			break;
		case PLAYER_VAN_HELSING:
			decidedLocation = helsingTurn(gameState, message, dracPos, myPos, round);
			break;
		case PLAYER_MINA_HARKER:
			decidedLocation = helsingTurn(gameState, message, dracPos, myPos, round);
			break;
	}
	// STOP DETERMINING A GOAL HERE
	if (myPos != NOWHERE) {
		nextLocation = findClosestLocationToTarget(myPos, decidedLocation, IAm, round, TRUE, TRUE, TRUE);
	} else {
		nextLocation = decidedLocation;
	}

	place = idToAbbrev(nextLocation);
	registerBestPlay(place, message);
}

static LocationID godalmTurn (HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round) {
	if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
		message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
		return myPos;
	} else if (dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) {
	// TODO: Make them do something useful here.
		message = givePresetMessage(gameState, SITTING_TIGHT);
		return BRUSSELS;
	} else {
		message = givePresetMessage(gameState, ON_THE_TRAIL);
		return dracPos;
	}
}

static LocationID sewardTurn (HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round) {
	if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
		message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
		return myPos;
	} else if (dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) {
		message = givePresetMessage(gameState, SITTING_TIGHT);
		return MADRID;
	} else {
		message = givePresetMessage(gameState, ON_THE_TRAIL);
		return dracPos;
	}
}

static LocationID helsingTurn (HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round) {
	if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
		message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
		return myPos;
	} else if (dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) {
		message = givePresetMessage(gameState, SITTING_TIGHT);
		return VENICE;
	} else {
		message = givePresetMessage(gameState, ON_THE_TRAIL);
		return dracPos;
	}
}

static LocationID minaTurn (HunterView gameState, char *message, LocationID dracPos, LocationID myPos, Round round) {
	if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
		message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
		return myPos;
	} else {
		message = givePresetMessage(gameState, CAMPING_THE_CASTLE);
		if (myPos == GALATZ) {
			return KLAUSENBURG;
		} else {
			return GALATZ;
		}
	}
}

static LocationID findClosestLocationToTarget(LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea) {
	// Firstly, all the reachable locations are stored.
	Set possiblePlacesSet = reachableLocations(from, player, round, road, rail, sea);
	LocationID decidedLocation = findClosestToTarget(possiblePlacesSet, from, to, player, round, road, rail, sea);
	disposeSet(possiblePlacesSet);
	return decidedLocation;
}

// Returns the last place Dracula was seen, or NOWHERE if they don't know.
static LocationID whereWasDraculaLastSeen(HunterView gameState) {
	LocationID dracTrail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
	for (int i = 0; i < TRAIL_SIZE; i++) {
		if (!(dracTrail[i] == CITY_UNKNOWN || dracTrail[i] == SEA_UNKNOWN || dracTrail[i] == NOWHERE)) {
			return dracTrail[i];
		}
	}
	return NOWHERE;
}


// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
static char *givePresetMessage(HunterView gameState, HunterMessage code) {
	switch (code) {
		case WAITING_FOR_THE_UNKNOWN:
			return "Systematic research time!";
		case HEALING:
			return "Heal it up.";
		case ON_THE_TRAIL:
			return "Bring it on, Bats.";
		case SITTING_TIGHT:
		default:
			return "Hopefully we'll find him.";
		case CAMPING_THE_CASTLE:
			return "At the castle.";
	}
}
