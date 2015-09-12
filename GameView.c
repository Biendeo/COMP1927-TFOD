// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Places.h"
// #include "Map.h"


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
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView gameView = malloc(sizeof(struct gameView));
	return gameView;
}
	 
	 
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView (GameView toBeDeleted) {
	free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

// Get the current score
int getScore(GameView currentView)
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
							LocationID trail[TRAIL_SIZE])
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
							   LocationID from, PlayerID player, Round round,
							   int road, int rail, int sea)
{
	//REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NULL;
}
