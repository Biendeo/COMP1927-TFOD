// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "Places.h"
#include "HunterView.h"


void decideHunterMove(HunterView gameState) {
	int turnNo = giveMeTheRound(gameState);
	// LocationID dracPos = whereIs(gameState, PLAYER_DRACULA); // UNUSED RIGHT NOW

	if (turnNo < 1) {
		registerBestPlay("CD","Let's begin at Castle Dracula");
	} else {
	   srand(time(NULL));
	   int amountMoves = 0;
	   LocationID *moves = whereCanIgo(gameState, &amountMoves, TRUE, FALSE,TRUE);
	   int decision = rand() % amountMoves;
	   char *choice = idToAbbrev(moves[decision]); 
	   registerBestPlay(choice,"I don't think he's showing up...");
	   free(moves);
	}
}