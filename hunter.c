// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

char *givePresetMessage(HunterView gameState);

void decideHunterMove(HunterView gameState) {
	// TODO ...
	// Replace the line below by something better
	registerBestPlay("GE", givePresetMessage(gameState));
}

// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
char *givePresetMessage(HunterView gameState) {
	switch (giveMeTheRound(gameState)) {
		default:
			return "The hunt is back in action.";
	}
}