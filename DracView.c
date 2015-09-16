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
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView (char *pastPlays, PlayerMessage messages[]) {
	DracView dracView = malloc(sizeof(struct dracView));
	dracView->g = newGameView(pastPlays, messages);
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
LocationID whereIs(DracView currentView, PlayerID player) {
	return getLocation(currentView->g, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
				 LocationID *start, LocationID *end) {
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
						 int *numTraps, int *numVamps) {
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
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
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea) {
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
						   PlayerID player, int road, int rail, int sea) {
	LocationID position = whereIs(currentView, player);
	// TODO: I don't think this charts everything.
	// A player may move up to three spaces by rail for example.
	return connectedLocations(currentView->g, numLocations, position, player, giveMeTheRound(currentView), road, rail, sea);
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
