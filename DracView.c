// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"

#include "Queue.h"


struct dracView {
	GameView g;
	LocationID vampLocation; //can have at most one minion vampire at any time
	int numTraps[NUM_MAP_LOCATIONS]; //an array that stores the number of traps for every LocationID
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView (char *pastPlays, PlayerMessage messages[]) {
	DracView dracView = malloc(sizeof(struct dracView));
	dracView->g = newGameView(pastPlays, messages);
	dracView->vampLocation = NOWHERE;
	int i;
	for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
		dracView->numTraps[i] = 0;
	}
	return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView (DracView toBeDeleted) {
	disposeGameView(toBeDeleted->g);
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
	getHistory(currentview->g, player, LocationID trail[TRAIL_SIZE]);
	*start = trail[1];
	*end = trail[0];
	return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
						 int *numTraps, int *numVamps) {
	if (where == currentView->vampLocation) {
		*numVamps = 1;
	} else {
		*numVamps = 0;
	}
	*numTraps = currentView->numTraps[where];
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
	//this function can only be called by Dracula himself
	assert(getCurrentPlayer(currentView->g) == PLAYER_DRACULA);
	LocationID *wcig;
	int i;
	Round round = giveMeTheRound(currentView);
	//In Round 0 Dracula can move to anywhere except for the hospital
	if (round == 0) {
		*numLocations = NUM_MAP_lOCATIONS - 1;
		wcig = (LocationID *)malloc(sizeof(LocationID) * (*numLocations));
		for (i = 0; i < *numLocations; i++) {
			if (i < ST_JOSEPH_AND_ST_MARYS) {
				wcig[i] = i;
			} else {
				wcig[i] = i + 1;
			}
		}
	} else {
		LocationID position = whereIs(currentView, PLAYER_DRACULA);
		wcig = connectedLocations(currentView->g, numLocations, position, PLAYER_DRACULA, round, road, rail, sea);
		//now that we've obtained the connected locations to Dracula's current position
		//we shall rule out the ones which are currently in his trail, with 2 exceptions
		//1) if Dracula doesn't have a HIDE move in his trail, 
		//   he may choose to stay in current position
		//2) if Dracula doesn't ahve a DOUBLE_BACK move in his trail, 
		//   he may choose to revisit an adjacent location in his trail
		//Note that HIDE and DOUBLE_BACK_? are labelled as special locations in Places.h
		LocationID *trail = (LocationID *)(malloc(sizeof(LocationID)*TRAIL_SIZE));
		giveMeTheTrail (currentView, PLAYER_DRACULA, trail);
		int doneHide = FALSE;
		int doneDoubleBack = FALSE;
		for (i = 0; i < TRAIL_SIZE; i++) {
			if (trail[i] == HIDE) {
				doneHide = TRUE;
			}
			if (trail[i] >= DOUBLE_BACK_1 && trail[i] <= DOUBLE_BACK_5) {
				doneDoubleBack = TRUE;
			}
		}
		if (doneHide == FALSE)
	}
	//nowhere Dracula can legally move to, automatically teleports back to CASTLE_DRACULA
	if (*numLocations == 0) {
		*wcig = CASTLE_DRACULA; //wcig points to a single location instead of an array of locations
		*numLocations + 1;
	}
	return wcig;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
						   PlayerID player, int road, int rail, int sea) {
	LocationID *wctg;
	if (player == PLAYER_DRACULA) {
		wctg = whereCanIgo(currentView, numLocations, road, sea);
	} else {
		LocationID position = whereIs(currentView, player);
		//DragView ADT is only used at Dracula's turns. Since Dracula moves last,
		//the function finds where hunters can go in their next (rather than current) round
		//also as a result we don't need to worry about where they can go in Round 0.
		Round nextRound = giveMeTheRound(currentView) + 1; 
		wctg = connectedLocations(currentView->g, numLocations, position, player, nextRound, road, rail, sea);
	}
	return wctg;
}



/*

"We Are The Champions"

I've paid my dues
Time after time.
I've done my sentence
But committed no crime.
And bad mistakes ‒
I've made a few.
I've had my share of sand kicked in my face
But I've come through.

(And I need just go on and on, and on, and on)

We are the champions, my friends,
And we'll keep on fighting 'til the end.
We are the champions.
We are the champions.
No time for losers
'Cause we are the champions of the world.

I've taken my bows
And my curtain calls
You brought me fame and fortune and everything that goes with it
I thank you all

But it's been no bed of roses,
No pleasure cruise.
I consider it a challenge before the whole human race
And I ain't gonna lose.

(And I need just go on and on, and on, and on)

We are the champions, my friends,
And we'll keep on fighting 'til the end.
We are the champions.
We are the champions.
No time for losers
'Cause we are the champions of the world.

We are the champions, my friends,
And we'll keep on fighting 'til the end.
We are the champions.
We are the champions.
No time for losers
'Cause we are the champions.

*/
