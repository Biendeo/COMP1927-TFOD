// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "DracView.h"
#include "Game.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"
#include "Set.h"

typedef enum DRACULA_MESSAGE {
	PLACING_TRAP,
	PLACING_VAMPIRE,
	HIDING,
	DOUBLE_BACKING,
	NOTHING
} DraculaMessage;

//finds closest location can move to on a path to 'to'
static LocationID getClosestLocationToTarget(DracView gameState, LocationID from, LocationID to, PlayerID player,
											 Round round, int road, int rail, int sea);
static char *givePresetMessage(DracView gameState, DraculaMessage code);	//make a message!
static void backUpStrat (DracView gameState);								//when all goes to hell, use this guaranteed to work (?)
static int trailFilter (LocationID *connections, LocationID *options,
								LocationID *trail, int numConnections);		//takes out trail moves from places drac can go, returns size of options
static void basicStartMove (DracView gameState);							//makes sure dracula registers at least one move a turn

void decideDraculaMove(DracView gameState) {
	basicStartMove(gameState);
	// TODO ...Replace the line below by something better
	Round roundNum = giveMeTheRound(gameState);
	LocationID GLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
	LocationID SLocation = whereIs(gameState, PLAYER_DR_SEWARD);
	LocationID HLocation = whereIs(gameState, PLAYER_VAN_HELSING);
	LocationID MLocation = whereIs(gameState, PLAYER_MINA_HARKER);
	//LocationID DLocation = whereIs(gameState, PLAYER_DRACULA);
	if (roundNum == 0) {
		// starting at Sofia: it's far from dracula's castle and has low chance of being cornered
		registerBestPlay("SO", givePresetMessage(gameState,0));
	} else {
		int numConnections = 0;
		LocationID *connections = whereCanIgo(gameState, &numConnections, TRUE, TRUE);
		LocationID *options = malloc(sizeof(LocationID)*numConnections); //will have all non-trail places drac can go
		LocationID trail[TRAIL_SIZE];
		giveMeTheTrail(gameState, PLAYER_DRACULA, trail);		      
		//will trail filter connections into options, and return size of options
		int numOptions = trailFilter(connections, options, trail, numConnections);

		// avoid running into a hunter (unless Dracula is surrounded by hunters)
		int surroundedByHunters = TRUE;
		for (int k = 0; k < numOptions; k++) {
			if (options[k] != GLocation || options[k] != SLocation ||
				options[k] != HLocation || options[k] != MLocation) {
				surroundedByHunters = FALSE;
			}
		}
		srand(time(NULL));
		int choiceIndex = rand() % numOptions;
		//keep rolling the choiceIndex until options[choiceIndex] is a hunter free place
		if (surroundedByHunters == FALSE) {
			while (options[choiceIndex] == GLocation || options[choiceIndex] == SLocation ||
				   options[choiceIndex] == HLocation || options[choiceIndex] == MLocation) {
				choiceIndex = rand() % numOptions;
			}
		}
		registerBestPlay(idToAbbrev(options[choiceIndex]), givePresetMessage(gameState, 0)); //saved 2 clock cycles, remove later if you like
		free(connections);
		free(options);
	} 
}

// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
static char *givePresetMessage(DracView gameState, DraculaMessage code) {
	switch (code) {
		case 0: return "Quiet as a bat."; break;
		case 1: return "Where is she?!"; break;
		case 2: return "Bats have nine lives."; break;
		case 3: return "If you hit me, you'll be charged for bat-tery."; break;
		case 4: return "You know I'm bat, I'm bat, I'm really really bat."; break;
		default: return "Mwuhahahaha"; break;
	}
}

static int trailFilter (LocationID *connections, LocationID *options, LocationID *trail, int numConnections) {
	int i,j;
	int k=0;
	int inTheTrail;
	for (i = 0; i < numConnections; i++) {
		inTheTrail = FALSE;
		for (j = 0; j < TRAIL_SIZE; j++) {
			if (connections[i] == trail[j]) {
				inTheTrail = TRUE;
				break;
			}
		}
		if (inTheTrail == TRUE) {
			continue;
		} else {
			options[k] = connections[i];
			k++;
		}
	}
	   // since k's value after the loop is the final index of the
	   //options array plus one, which is the same as
	   // the number of elements in the options array
	return k;
}

static void basicStartMove (DracView gameState) {
	registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_DRACULA)), "botman");
}

/*static LocationID getClosestLocationToTarget(DracView gameState, LocationID from, LocationID to, PlayerID player, Round round, int road, int rail, int sea) {
	// Firstly, all the reachable locations are stored.
	Set possiblePlacesSet = reachableLocations(from, player, round, road, rail, sea);

	if (player == PLAYER_DRACULA) {
		LocationID dracTrail[TRAIL_SIZE];
		// We just check to see if he's not trying to move into his trail.
		giveMeTheTrail(gameState, player, dracTrail);
		for (int i = 0; i < TRAIL_SIZE; i++) {
			// Don't let this part run. We should always check to see if to is not in his trail
			// before we even get here.
			if (dracTrail[i] == to) {
				disposeSet(possiblePlacesSet);
				return NOWHERE;
			}
			// If his trail is a possible move, we rule it out.
			if (isElem(possiblePlacesSet, dracTrail[i])) {
				setRemove(possiblePlacesSet, dracTrail[i]);
			}
		}
	}

	LocationID decidedLocation = findClosestToTarget(possiblePlacesSet, from, to, player, round, road, rail, sea);
	disposeSet(possiblePlacesSet);
	return decidedLocation;
}*/

/*static void backUpStrat (DracView gameState) {
	Round roundNum = giveMeTheRound(gameState);
	if (roundNum % 7 == 0) {
		registerBestPlay("BR", givePresetMessage(gameState,0));
	} else if (roundNum % 7 == 1) {
		registerBestPlay("PR", givePresetMessage(gameState,0));
	} else if (roundNum % 7 == 2) {
		registerBestPlay("VI", givePresetMessage(gameState,0));
	} else if (roundNum % 7 == 3) {
		registerBestPlay("MU", givePresetMessage(gameState,0));
	} else if (roundNum % 7 == 4) {
		registerBestPlay("NU", givePresetMessage(gameState,0));
	} else if (roundNum % 7 == 5) {
		registerBestPlay("FR", givePresetMessage(gameState,0));
	} else {
		registerBestPlay("LI", givePresetMessage(gameState,0));
	}
} */
