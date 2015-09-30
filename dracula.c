// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

char *givePresetMessage(DracView gameState);

void decideDraculaMove(DracView gameState) {
	// TODO ...
	// Replace the line below by something better
	registerBestPlay("CD", givePresetMessage(gameState));
}

// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
char *givePresetMessage(DracView gameState) {
	switch (giveMeTheRound(gameState)) {
		case 0:
			return "Quiet as a bat.";
		case 1:
			return "Where is she?!";
		case 2:
			return "Bats have nine lives.";
		case 3:
			return "If you hit me, you'll be charged for bat-tery.";
		case 4:
			return "You know I'm bat, I'm bat, I'm really really bat.";
		case 5:
			return "Bat to the bone.";
		case 13:
			return "Fly my pretties!";
		default:
			return "Mwuhahahaha";
	}
}