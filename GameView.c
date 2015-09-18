// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "Map.h"

#include "Queue.h"
#include "Set.h"


// This struct stores data that is common to each player.
struct playerData {

	// This tracks the ID of the player (using the PlayerID typedef in Globals.h).
	PlayerID ID;

	// This tracks the amount of health a player has. Dracula will have a
	// different max amount.
	char health;

	// This tracks where on the map the player is.
	LocationID location;

	// This tracks each player's last 6 moves.
	Trail trail;
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
}; 

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]) {
	GameView gameView = malloc(sizeof(struct gameView));

	// TODO: Interpret the pastPlays and figure out the statistics.

	gameView->whoseTurn = PLAYER_LORD_GODALMING;
	gameView->turnNumber = 0;
	gameView->score = GAME_START_SCORE;

	for (int i = 0; i < NUM_PLAYERS; i++) {
		// This is a bit lazy, but it's short and easy.
		gameView->player[i].ID = i;
		if (i != PLAYER_DRACULA) {
			gameView->player[i].health = GAME_START_HUNTER_LIFE_POINTS;
			gameView->player[i].location = UNKNOWN_LOCATION;
		} else {
			gameView->player[i].health = GAME_START_BLOOD_POINTS;
			gameView->player[i].location = CASTLE_DRACULA;
		}
		gameView->player[i].trail = newTrail(TRAIL_SIZE);
	}

	char *currentPlayMarker = pastPlays;
	char currentPlay[8] = {'\0'};
	while (currentPlayMarker != NULL) {
		memcpy(currentPlay, currentPlayMarker, 8);
		// TODO: Compute each part of input.

		if (currentPlay[7] == '\0') break;
		currentPlayMarker += 8;
	}

	return gameView;
}
	 
	 
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView (GameView toBeDeleted) {
	for (int i = 0; i < NUM_PLAYERS; i++) {
		disposeTrail(toBeDeleted->player[i].trail);
	}
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
	for (int i = 0; i < TRAIL_SIZE; i++) {
		trail[i] = showElement(currentView->player[player].trail, i);
	}
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations (GameView currentView, int *numLocations,
							   LocationID from, PlayerID player, Round round,
							   int road, int rail, int sea) {
	if (from == UNKNOWN_LOCATION || from == CITY_UNKNOWN || from == SEA_UNKNOWN) {
		LocationID *connectedLocations = malloc(sizeof(LocationID));
		*connectedLocations = from;
		return connectedLocations;
	}
	Map map = newMap();
	VList node = map->connections[from];
	*numLocations = 1;
	LocationID *connectedLocations = malloc(sizeof(LocationID) * *numLocations);
	connectedLocations[0] = from;
	
	while (node != NULL) {
		// TODO: Add a Dracula check for hospitals.
		// There is no check for the trail though.
		if (road == 1 && node->type == ROAD) {
			*numLocations += 1;
			connectedLocations = realloc(connectedLocations, sizeof(LocationID) * *numLocations);
			connectedLocations[*numLocations - 1] = node->v;
		} else if (rail == 1 && node->type == RAIL) {
			// TODO: We're going to need a set, and check recursively for this.
			*numLocations += 1;
			connectedLocations = realloc(connectedLocations, sizeof(LocationID) * *numLocations);
			connectedLocations[*numLocations - 1] = node->v;
		} else if (sea == 1 && node->type == SEA) {
			*numLocations += 1;
			connectedLocations = realloc(connectedLocations, sizeof(LocationID) * *numLocations);
			connectedLocations[*numLocations - 1] = node->v;
		}
		node = node->next;
	}

	disposeMap(map);
	return connectedLocations;
}