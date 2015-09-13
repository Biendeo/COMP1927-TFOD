// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "Map.h"


// This struct stores data that is common to each player.
struct playerData {

	// This tracks the ID of the player (using the PlayerID typedef in Globals.h).
	PlayerID ID;

	// This tracks the amount of health a player has. Dracula will have a
	// different max amount.
	char health;

	// This tracks where on the map the player is.
	LocationID location;
};


struct gameView {

	// Each of these stores data types that are common to all players.
	// TODO: Vote on the names?
	// TODO: Should this be an array? (could be convenient as the player IDs go
	// from 0 to 4.
	struct playerData LG;
	struct playerData DS;
	struct playerData VH;
	struct playerData MH;
	struct playerData DR;

	// This tracks the score of the game.
	int score;

	// This tracks the turn number of the game.
	Round turnNumber;

	// This tracks whose turn it is.
	PlayerID whoseTurn;

	// TODO: Storing the Dracula path? It's not in the header, so we may not need
	// store it here.
}; 

// Creates a new GameView to summarise the current state of the game
GameView newGameView (char *pastPlays, PlayerMessage messages[]) {
	GameView gameView = malloc(sizeof(struct gameView));
	return gameView;
}
	 
	 
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView (GameView toBeDeleted) {
	free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound (GameView currentView) {
	return currentView->turnNumber;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer (GameView currentView) {
	return currentView->whoseTurn;
}

// Get the current score
int getScore (GameView currentView) {
	return currentView->score;
}

// Get the current health points for a given player
int getHealth (GameView currentView, PlayerID player) {
	switch (player) {
		case PLAYER_LORD_GODALMING:
			return currentView->LG.health;
		case PLAYER_DR_SEWARD:
			return currentView->DS.health;
		case PLAYER_VAN_HELSING:
			return currentView->VH.health;
		case PLAYER_MINA_HARKER:
			return currentView->MH.health;
			// TODO: Remove this when Dracula is moved to DracView.
		case PLAYER_DRACULA:
			return currentView->DR.health;
	}
}

// Get the current location id of a given player
LocationID getLocation (GameView currentView, PlayerID player) {
	switch (player) {
		case PLAYER_LORD_GODALMING:
			return currentView->LG.location;
		case PLAYER_DR_SEWARD:
			return currentView->DS.location;
		case PLAYER_VAN_HELSING:
			return currentView->VH.location;
		case PLAYER_MINA_HARKER:
			return currentView->MH.location;
		// TODO: Remove this when Dracula is moved to DracView.
		case PLAYER_DRACULA:
			return currentView->DR.location;
	}
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory (GameView currentView, PlayerID player,
							LocationID trail[TRAIL_SIZE]) {
	// TODO: This when the queue is done.
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations (GameView currentView, int *numLocations,
							   LocationID from, PlayerID player, Round round,
							   int road, int rail, int sea) {
	
	return NULL;
}
