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
			if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			} else if (dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) {
				// TODO: Make them do something useful here.
				decidedLocation = BRUSSELS;
				message = givePresetMessage(gameState, SITTING_TIGHT);
			} else {
				decidedLocation = dracPos;
				message = givePresetMessage(gameState, ON_THE_TRAIL);
			}
			break;
		case PLAYER_DR_SEWARD:
			if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			} else if (dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) {
				decidedLocation = MADRID;
				message = givePresetMessage(gameState, SITTING_TIGHT);
			} else {
				decidedLocation = dracPos;
				message = givePresetMessage(gameState, ON_THE_TRAIL);
			}
			break;
		case PLAYER_VAN_HELSING:
			if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			} else if (dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) {
				decidedLocation = VENICE;
				message = givePresetMessage(gameState, SITTING_TIGHT);
			} else {
				decidedLocation = dracPos;
				message = givePresetMessage(gameState, ON_THE_TRAIL);
			}
			break;
		case PLAYER_MINA_HARKER:
			if ((dracPos == NOWHERE || (dracPos >= DOUBLE_BACK_1 && dracPos <= DOUBLE_BACK_5) || dracPos == HIDE) && round > 5) {
				decidedLocation = myPos;
				message = givePresetMessage(gameState, WAITING_FOR_THE_UNKNOWN);
			} else {
				if (myPos == GALATZ) {
					decidedLocation = KLAUSENBURG;
				} else {
					decidedLocation = GALATZ;
				}
				message = givePresetMessage(gameState, CAMPING_THE_CASTLE);
			}
			break;
	}

	// STOP DETERMINING A GOAL HERE

	if (myPos != NOWHERE) {
		nextLocation = findClosestLocationToTarget(myPos, decidedLocation, IAm, round, TRUE, FALSE, TRUE);
	} else {
		nextLocation = decidedLocation;
	}

	place = idToAbbrev(nextLocation);
	registerBestPlay(place, message);
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
