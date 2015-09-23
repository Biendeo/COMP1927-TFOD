// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "Map.h"

#include "Queue.h"
#include "Set.h"

#define LONG_TRAIL_SIZE 11

int IDToType(GameView g, LocationID p);
LocationID getTrueLocation(GameView g, LocationID p);
LocationID AbbrevToID(char *abbrev);

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
		gameView->player[i].ID = i;
		if (i != PLAYER_DRACULA) {
			gameView->player[i].health = GAME_START_HUNTER_LIFE_POINTS;
		} else {
			gameView->player[i].health = GAME_START_BLOOD_POINTS;
		}
		gameView->player[i].location = NOWHERE;
		gameView->player[i].trail = newTrail(LONG_TRAIL_SIZE);
	}

	// If pastPlays is empty, we just return early.
	if (strcmp(pastPlays, "") == 0) {
		return gameView;
	}

	char *currentPlayMarker = pastPlays;
	char currentPlay[8] = {'\0'};
	char givenLocation[3] = {'\0'};
	while (currentPlayMarker != NULL) {
		memcpy(currentPlay, currentPlayMarker, 8);

		// As long as we track whoseTurn properly, we shouldn't need to read
		// the first character.

		// We isolate the location part for easy access..
		memcpy(givenLocation, currentPlay + 1, 2);

		// If a hunter has been dead, then he'll be back to life.
		if (gameView->whoseTurn != PLAYER_DRACULA && gameView->player[gameView->whoseTurn].health == 0) {
			gameView->player[gameView->whoseTurn].health = GAME_START_HUNTER_LIFE_POINTS;
		}

		// We also push the location to that person's trail.
		prepend(gameView->player[gameView->whoseTurn].trail, AbbrevToID(givenLocation));

		// We get the location and set the player's location to there.
		gameView->player[gameView->whoseTurn].location = getTrueLocation(gameView, AbbrevToID(givenLocation));


		if (gameView->whoseTurn != PLAYER_DRACULA) {
			// If a hunter stays at the same city, they gain health from rest.
			if (showElement(gameView->player[gameView->whoseTurn].trail, 0) == showElement(gameView->player[gameView->whoseTurn].trail, 1)) {
				gameView->player[gameView->whoseTurn].health += LIFE_GAIN_REST;
			}
		} else {
			// If Dracula ends a turn at sea, he loses health.
			if (IDToType(gameView, AbbrevToID(givenLocation)) == SEA) {
				gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;
				// If Dracula ends a turn at his castle, he gains health.
			} else if (gameView->player[PLAYER_DRACULA].location == CASTLE_DRACULA) {
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
						// Trap data handled by DracView.
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
				// If the hunter is dead, they don't do anything else.
				if (gameView->player[gameView->whoseTurn].health <= 0) {
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

		if (gameView->whoseTurn != PLAYER_DRACULA) {
			// When a hunter dies, they're moved to the hospital.
			if (gameView->player[gameView->whoseTurn].health <= 0) {
				gameView->player[gameView->whoseTurn].location = ST_JOSEPH_AND_ST_MARYS;
				gameView->player[gameView->whoseTurn].health = 0;
				gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
			}
			// Hunters can't exceed their maximum life.
			if (gameView->player[gameView->whoseTurn].health > GAME_START_HUNTER_LIFE_POINTS) {
				gameView->player[gameView->whoseTurn].health = GAME_START_HUNTER_LIFE_POINTS;
			}
		} else {
			if (gameView->player[gameView->whoseTurn].location == CASTLE_DRACULA) {
				gameView->player[gameView->whoseTurn].health += LIFE_GAIN_CASTLE_DRACULA;
			}
			// Dracula always lowers the score each turn.
			gameView->score -= SCORE_LOSS_DRACULA_TURN;
		}

		// Then increment whoseTurn (moving it back down if needed).
		// This anticipates the next turn, so it should return the right turn to the AI.
		gameView->whoseTurn++;
		if (gameView->whoseTurn >= NUM_PLAYERS) {
			gameView->whoseTurn = 0;
			gameView->turnNumber += 1;
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
		return UNKNOWN_LOCATION;
	} else {
		return showElement(currentView->player[player].trail, 0);
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

// ----------------------------------------
// THESE FUNCTIONS BELOW ARE NOT IN THE ADT
// ----------------------------------------

// This converts a location ID to a type. Unlike the Places.c version, it
// also returns unknown places.
int IDToType(GameView g, LocationID p) {
	switch (p) {
		case CITY_UNKNOWN:
			return LAND;
		case SEA_UNKNOWN:
			return SEA;
		case HIDE:
		case DOUBLE_BACK_1:
		case DOUBLE_BACK_2:
		case DOUBLE_BACK_3:
		case DOUBLE_BACK_4:
		case DOUBLE_BACK_5:
			return IDToType(g, getTrueLocation(g, p));
		case TELEPORT:
			return LAND;
		default:
			return idToType(p);
	}
}

// This runs through Dracula's special moves to determine his true location.
LocationID getTrueLocation(GameView g, LocationID p) {
	switch (p) {
		case HIDE:
		case DOUBLE_BACK_1:
		case DOUBLE_BACK_2:
		case DOUBLE_BACK_3:
		case DOUBLE_BACK_4:
		case DOUBLE_BACK_5:
			for (int i = 0; i < LONG_TRAIL_SIZE; i++) {
				if (showElement(g->player[g->whoseTurn].trail, i) == p) {
					switch (p) {
						case HIDE:
							return getTrueLocation(g, showElement(g->player[g->whoseTurn].trail, i + 1));
						case DOUBLE_BACK_1:
							return getTrueLocation(g, showElement(g->player[g->whoseTurn].trail, i + 1));
						case DOUBLE_BACK_2:
							return getTrueLocation(g, showElement(g->player[g->whoseTurn].trail, i + 2));
						case DOUBLE_BACK_3:
							return getTrueLocation(g, showElement(g->player[g->whoseTurn].trail, i + 3));
						case DOUBLE_BACK_4:
							return getTrueLocation(g, showElement(g->player[g->whoseTurn].trail, i + 4));
						case DOUBLE_BACK_5:
							return getTrueLocation(g, showElement(g->player[g->whoseTurn].trail, i + 5));
					}
				}
			}
		case TELEPORT:
			return CASTLE_DRACULA;
		default:
			return p;
	}
}

// This converts an abbreviation to a location ID. Unlike the Places.c version,
// it also converts non-city places.
LocationID AbbrevToID(char *abbrev) {
	if (strcmp(abbrev, "C?") == 0) {
		return CITY_UNKNOWN;
	} else if (strcmp(abbrev, "S?") == 0) {
		return SEA_UNKNOWN;
	} else if (strcmp(abbrev, "HI") == 0) {
		return HIDE;
	} else if (strcmp(abbrev, "D1") == 0) {
		return DOUBLE_BACK_1;
	} else if (strcmp(abbrev, "D2") == 0) {
		return DOUBLE_BACK_2;
	} else if (strcmp(abbrev, "D3") == 0) {
		return DOUBLE_BACK_3;
	} else if (strcmp(abbrev, "D4") == 0) {
		return DOUBLE_BACK_4;
	} else if (strcmp(abbrev, "D5") == 0) {
		return DOUBLE_BACK_5;
	} else if (strcmp(abbrev, "TP") == 0) {
		return TELEPORT;
	} else {
		return abbrevToID(abbrev);
	}
}