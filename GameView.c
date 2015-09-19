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

	gameView->whoseTurn = PLAYER_LORD_GODALMING;
	gameView->turnNumber = 0;
	gameView->score = GAME_START_SCORE;

	for (int i = 0; i < NUM_PLAYERS; i++) {
		// This is a bit lazy, but it's short and easy.
		gameView->player[i].ID = i;
		if (i != PLAYER_DRACULA) {
			gameView->player[i].health = GAME_START_HUNTER_LIFE_POINTS;
gameView->player[i].location = NOWHERE;
		} else {
		gameView->player[i].health = GAME_START_BLOOD_POINTS;
		gameView->player[i].location = CASTLE_DRACULA;
		}
		gameView->player[i].trail = newTrail(TRAIL_SIZE + 5);
	}

	char *currentPlayMarker = pastPlays;
	char currentPlay[8] = {'\0'};
	char givenLocation[3] = {'0'};
	while (currentPlayMarker != NULL) {
		memcpy(currentPlay, currentPlayMarker, 8);

		// As long as we track whoseTurn properly, we shouldn't need to read
		// the first character.

		// We isolate the location part for easy access.
		memcpy(currentPlay[1], givenLocation, 2);

		// We get the location and set the player's location to there.
		gameView->player[gameView->whoseTurn].location = abbrevToID(givenLocation);

		// We also push the location to that person's trail.
		prepend(gameView->player[gameView->whoseTurn].trail, abbrevToID(givenLocation));

		if (gameView->whoseTurn != PLAYER_DRACULA) {
			// If a hunter stays at the same city, they gain health from rest.
			if (showElement(gameView->player[gameView->whoseTurn].trail, 0) == showElement(gameView->player[gameView->whoseTurn].trail, 1)) {
				gameView->player[gameView->whoseTurn].health += LIFE_GAIN_REST;
			}
		} else {
			// If Dracula ends a turn at sea, he loses health.
			if (idToType(abbrevToID(givenLocation)) == SEA) {
				gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;
				// If Dracula ends a turn at his castle, he gains health.
			} else if (abbrevToID(givenLocation) == CASTLE_DRACULA) {
				gameView->player[PLAYER_DRACULA].health += LIFE_GAIN_CASTLE_DRACULA;
			}
		}

		// Then we parse each of the next four characters.
		// TRAPS ARE TRACKED IN DRACVIEW, SO THERE'S EMPTY BITS HERE.
		for (int i = 3; i < 7; i++) {
			// HUNTERS
			if (gameView->whoseTurn != PLAYER_DRACULA) {
				switch (currentPlay[i]) {
					// TRAP FOUND
					case 'T':
						// Vampire data handled by DracView.
						gameView->player[gameView->whoseTurn].health -= LIFE_LOSS_TRAP_ENCOUNTER;
						break;
					// VAMPIRE FOUND
					case 'V':
						// Vampire data handled by DracView.
						break;
					// DRACULA FOUND
					case 'D':
						gameView->player[gameView->whoseTurn].health -= LIFE_LOSS_DRACULA_ENCOUNTER;
						gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_HUNTER_ENCOUNTER;
						break;
					// NOTHING
					default:
						break;
				}
				// DRACULA
			} else {
				if (i == 3 || i == 4) {
					switch (currentPlay[i]) {
						// TRAP PLACED
						case 'T':
							break;
						// VAMPIRE PLACED
						case 'V':
							break;
						// NOTHING
						default:
							break;
					}
				} else if (i == 5) {
					switch (currentPlay[i]) {
						// TRAP LEAVES
						case 'M':
							// Trap data handled by DracView.
							break;
						// VAMPIRE MATURES
						case 'V':
							// Vampire data handled by DracView.
							gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
							break;
						// NOTHING
						default:
							break;
					}
				}
			}
		}

		// TODO: Fix this logic bit.
		if (gameView->whoseTurn != PLAYER_DRACULA) {
			// When a hunter dies, they're moved to the hospital.
			if (gameView->player[gameView->whoseTurn].health <= 0 && gameView->player[gameView->whoseTurn].location != ST_JOSEPH_AND_ST_MARYS) {
				gameView->player[gameView->whoseTurn].location = ST_JOSEPH_AND_ST_MARYS;
				gameView->player[gameView->whoseTurn].health = 0;
				gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
			// If they've spent their turn there, they heal.
			} else if (gameView->player[gameView->whoseTurn].health == 0 && gameView->player[gameView->whoseTurn].location == ST_JOSEPH_AND_ST_MARYS) {
				gameView->player[gameView->whoseTurn].health = GAME_START_HUNTER_LIFE_POINTS;
			}
			// Hunters can't exceed their maximum life.
			if (gameView->player[gameView->whoseTurn].health > GAME_START_HUNTER_LIFE_POINTS) {
				gameView->player[gameView->whoseTurn].health = GAME_START_HUNTER_LIFE_POINTS;
			}
		} else {
			// Dracula always lowers the score each turn.
			gameView->score -= SCORE_LOSS_DRACULA_TURN;
		}

		// Then increment whoseTurn (moving it back down if needed).
		// This anticipates the next turn, so it should return the right turn to the AI.
		gameView->whoseTurn++;
		if (gameView->whoseTurn >= NUM_PLAYERS) {
			gameView->whoseTurn = 0;
		}

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
	if (from == NOWHERE || from == CITY_UNKNOWN || from == SEA_UNKNOWN) {
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