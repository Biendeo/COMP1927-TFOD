// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "Map.h"

#include "Queue.h"


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

	// This tracks Dracula's trail.
	Trail trail;
}; 

// Creates a new GameView to summarise the current state of the game
GameView newGameView (char *pastPlays, PlayerMessage messages[]) {
	GameView gameView = malloc(sizeof(struct gameView));
	gameView->trail = newTrail(TRAIL_SIZE);

	// TODO: Interpret the pastPlays and figure out the statistics.

	gameView->whoseTurn = PLAYER_LORD_GODALMING;
	gameView->turnNumber = 0;
	gameView->score = GAME_START_SCORE;

	// TODO: Make this a bit cleaner.
	gameView->LG.ID = PLAYER_LORD_GODALMING;
	gameView->LG.health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->LG.location = UNKNOWN_LOCATION;

	gameView->DS.ID = PLAYER_DR_SEWARD;
	gameView->DS.health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->DS.location = UNKNOWN_LOCATION;

	gameView->VH.ID = PLAYER_VAN_HELSING;
	gameView->VH.health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->VH.location = UNKNOWN_LOCATION;

	gameView->MH.ID = PLAYER_MINA_HARKER;
	gameView->MH.health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->MH.location = UNKNOWN_LOCATION;

	gameView->DR.ID = PLAYER_DRACULA;
	gameView->DR.health = GAME_START_BLOOD_POINTS;
	gameView->DR.location = CASTLE_DRACULA;

	return gameView;
}
	 
	 
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView (GameView toBeDeleted) {
	disposeTrail(toBeDeleted->trail);
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
		default:
			return -1;
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
		default:
			return -1;
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
	Map map = newMap();
	// TODO: Any of this.
	destroyMap(map);
	return NULL;
}
