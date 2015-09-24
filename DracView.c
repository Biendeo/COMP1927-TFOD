// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"

#include "Queue.h"
#include "Set.h"


struct dracView {
	GameView g;

	LocationID vampLocation; //can have at most one minion vampire at any time
	Trail traps;
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView (char *pastPlays, PlayerMessage messages[]) {
	DracView dracView = malloc(sizeof(struct dracView));
	dracView->g = newGameView(pastPlays, messages);
	dracView->vampLocation = NOWHERE;
	dracView->traps = newTrail(TRAIL_SIZE);

	// If pastPlays is empty, we just return early.
	if (strcmp(pastPlays, "") == 0) {
		return dracView;
	}

	char *currentPlayMarker = pastPlays;
	char currentPlay[8] = {'\0'};
	char givenLocation[3] = {'\0'};
	LocationID currentPlace = NOWHERE;
	while (currentPlayMarker != NULL) {
		memcpy(currentPlay, currentPlayMarker, 8);

		if (currentPlay[0] == 'D') {
			// We isolate the location part for easy access..
			memcpy(givenLocation, currentPlay + 1, 2);
			
			currentPlace = abbrevToID(givenLocation);
			
			// Then we parse each of the next four characters.
			// TRAPS ARE TRACKED IN DRACVIEW, SO THERE'S EMPTY BITS HERE.
			for (int i = 3; i < 6; i++) {
				if (i == 3 || i == 4) {
					switch (currentPlay[i]) {
						// TRAP PLACED
						case 'T':
							prepend(dracView->traps, currentPlace);
							break;
							// VAMPIRE PLACED
						case 'V':
							dracView->vampLocation = currentPlace;
							break;
							// NOTHING
						default:
							break;
					}
				} else if (i == 5) {
					switch (currentPlay[i]) {
						// TRAP LEAVES
						case 'M':
							while (showElement(dracView->traps, TRAIL_SIZE - 1) != NOWHERE) {
								prepend(dracView->traps, NOWHERE);
							}
							prepend(dracView->traps, NOWHERE);
							break;
							// VAMPIRE MATURES
						case 'V':
							dracView->vampLocation = NOWHERE;
							break;
							// NOTHING
						default:
							break;
					}
				}
			}
		}

		if (currentPlay[7] == '\0') break;
		currentPlayMarker += 8;
	}
	return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView (DracView toBeDeleted) {
	disposeGameView(toBeDeleted->g);
	disposeTrail(toBeDeleted->traps);
	free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView) {
	return getRound(currentView->g);
}

// Get the current score
int giveMeTheScore(DracView currentView) {
	return getScore(currentView->g);
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player) {
	return getHealth(currentView->g, player);
}

// Get the current location id of a given player
// Dracula knows where everyone is
// This function may need correction. getLocation() in GameView may return a special location
// such as HIDE, DOUBLE_BACK_? and TELEPORT but this function shall return the exact location
LocationID whereIs(DracView currentView, PlayerID player) {
	return getLocation(currentView->g, player);
	//use getHistory instead to identify exact location when getLocation() returns a special location
}

// Get the most recent move of a given player
// may need modification when Queue ADT is confirmed
void lastMove(DracView currentView, PlayerID player,
				 LocationID *start, LocationID *end) {
	LocationID *trail = malloc(sizeof(LocationID) * TRAIL_SIZE);
	getHistory(currentView->g, player, trail);
	*start = trail[1];
	*end = trail[0];
	free(trail);
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
						 int *numTraps, int *numVamps) {
	if (where == currentView->vampLocation) {
		*numVamps = 1;
	} else {
		*numVamps = 0;
	}

	*numTraps = 0;
	for (int i = 0; i < TRAIL_SIZE; i++) {
		if (showElement(currentView->traps, i) == where) {
			*numTraps += 1;
		}
	}
	return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
							LocationID trail[TRAIL_SIZE]) {
	getHistory(currentView->g, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
// Question 1: should hide and double back be considered here? 
// Question 2: does Dracula's teleportation only happen when there is no legal move possible?
// (currently the function is implemented assuming positive answers to both questions)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea) {
	return whereCanTheyGo(currentView, numLocations, PLAYER_DRACULA, road, 0, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations, PlayerID player, int road, int rail, int sea) {
	// This will get ALL the connected locations.
	LocationID *arrConnected = connectedLocations(currentView->g, numLocations, getLocation(currentView->g, player), player, getRound(currentView->g), road, rail, sea);

	// The only trick is that Dracula must remove his trail.
	if (player == PLAYER_DRACULA) {
		Set setConnected = copyArrayToSet(arrConnected, *numLocations);

		LocationID pastSix[TRAIL_SIZE] = {0};
		giveMeTheTrail(currentView, player, pastSix);

		for (int i = 0; i < TRAIL_SIZE; i++) {
			if (isElem(setConnected, pastSix[i])) {
				setRemove(setConnected, pastSix[i]);
				*numLocations -= 1;
			}
		}

		if (isElem(setConnected, ST_JOSEPH_AND_ST_MARYS)) {
			setRemove(setConnected, ST_JOSEPH_AND_ST_MARYS);
			*numLocations -= 1;
		}

		free(arrConnected);
		arrConnected = copySetToArray(setConnected);
	}
	return arrConnected;
}