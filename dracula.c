// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "DracView.h"

char *givePresetMessage(DracView gameState);

void decideDraculaMove(DracView gameState) {
	// TODO ...
	// Replace the line below by something better
	Round roundNum = giveMeTheRound(gameState);
	LocationID GLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
	LocationID SLocation = whereIs(gameState, PLAYER_DR_SEWARD);
	LocationID HLocation = whereIs(gameState, PLAYER_VAN_HELSING);
	LocationID MLocation = whereIs(gameState, PLAYER_MINA_HARKER);
	LocationID DLocation = whereIs(gameState, PLAYER_DRACULA);
	if (roundNum == 0) {
		// starting at Sofia since it's far from dracula's castle and
		// not on the edge of the map so dracula is less likely to be cornered
		registerBestPlay("SO", givePresetMessage(gameState));
	} else {
		// since this AI doesn't know how to self-heal yet, it's probably better
		// to avoid sea travelling and confronting hunters whenever possible
		// Note: current location is always in the array given by whereCanIgo()
		// but HIDE is not always available
		int *numOptions = 0;
		LocationID *options = whereCanIgo(gameState, numOptions, TRUE, FALSE);
		// If the array contains only current location, explore sea options
		if (*numOptions <= 1) {
			LocationID *options = whereCanIgo(gameState, numOptions, TRUE, TRUE);	
		}
		// If there there is no sea options either, staying in current location is
		// the only option (if even HIDE is unavailable game engine would have automatically
		// teleported Dracula back to his castle???)
		if (*numOptions <= 1) {
			char * currentLocation = idToAbbrev(DLocation);
			registerBestPlay(currentLocation, givePresetMessage(gameState));
		} else {
			// avoid running into a hunter (unless Dracula is surrounded by hunters)
			int counter;
			int surroundedByHunters = TRUE;
			for (counter=1; counter < *numOptions; counter++) {
				if (options[counter] != GLocation || options[counter] != SLocationS ||
					options[counter] != HLocation || options[counter] != MLocation) {
						surroundedByHunters = FALSE;
					}
			}
			int realnumOptions = *numOptions - 1; //excluding the current location "option"
			srand(time(NULL));
			int choiceIndex = rand() % realnumOptions + 1;
			// keep rolling the choiceIndex if we know there is at least one connectedLocation that's
			// free of hunters
			if (surroundedByHunters == FALSE) {
				while (options[choiceIndex] == GLocation || options[choiceIndex] == SLocation ||
					   options[choiceIndex] == HLocation || options[choiceIndex] == MLocation) {
					choiceIndex = rand() % realnumOptions + 1;
				}
			}
			char * choice = idToAbbrev(options[choiceIndex]);
			registerBestPlay(choice, givePresetMessage(gameState));
		}
	}
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
