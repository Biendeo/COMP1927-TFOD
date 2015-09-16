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
	struct playerData player[NUM_PLAYERS];

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
	gameView->player[0].ID = PLAYER_LORD_GODALMING;
	gameView->player[0].health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->player[0].location = UNKNOWN_LOCATION;

	gameView->player[1].ID = PLAYER_DR_SEWARD;
	gameView->player[1].health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->player[1].location = UNKNOWN_LOCATION;

	gameView->player[2].ID = PLAYER_VAN_HELSING;
	gameView->player[2].health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->player[2].location = UNKNOWN_LOCATION;

	gameView->player[3].ID = PLAYER_MINA_HARKER;
	gameView->player[3].health = GAME_START_HUNTER_LIFE_POINTS;
	gameView->player[3].location = UNKNOWN_LOCATION;

	gameView->player[4].ID = PLAYER_DRACULA;
	gameView->player[4].health = GAME_START_BLOOD_POINTS;
	gameView->player[4].location = CASTLE_DRACULA;

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
	if (player >= NUM_PLAYERS || player < 0) {
		return -1;
	} else {
		return currentView->player[player].health;
	}
}

// Get the current location id of a given player
LocationID getLocation (GameView currentView, PlayerID player) {
	if (player >= NUM_PLAYERS || player < 0) {
		return -1;
	} else {
		return currentView->player[player].location;
	}
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory (GameView currentView, PlayerID player,
							LocationID trail[TRAIL_SIZE]) {
	// TODO: Does each player have a trail?
	// I'm just assuming it's Dracula every time.
	for (int i = 0; i < TRAIL_SIZE; i++) {
		trail[i] = showElement(currentView->trail, i);
	}
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations (GameView currentView, int *numLocations,
							   LocationID from, PlayerID player, Round round,
							   int road, int rail, int sea) {
	Map map = newMap();
	VList node = map->connections[from];
	*numLocations = 0;
	LocationID *connectedLocations = malloc(sizeof(LocationID) * *numLocations);
	
	while (node != NULL) {
		if (road == 1 && node->type == ROAD) {
			*numLocations++;
			connectedLocations = realloc(connectedLocations, sizeof(LocationID) * *numLocations);
			connectedLocations[*numLocations - 1] = node->v;
		} else if (rail == 1 && node->type == RAIL) {
			*numLocations++;
			connectedLocations = realloc(connectedLocations, sizeof(LocationID) * *numLocations);
			connectedLocations[*numLocations - 1] = node->v;
		} else if (sea == 1 && node->type == SEA) {
			*numLocations++;
			connectedLocations = realloc(connectedLocations, sizeof(LocationID) * *numLocations);
			connectedLocations[*numLocations - 1] = node->v;
		}
		node = node->next;
	}

	disposeMap(map);
	return connectedLocations;
}