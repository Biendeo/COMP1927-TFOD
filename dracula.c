// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "DracView.h"
#include "Game.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"
#include "Set.h"

typedef enum DRACULA_MESSAGE {
	PLACING_TRAP,
	PLACING_VAMPIRE,
	HIDING,
	DOUBLE_BACKING,
	NOTHING
} DraculaMessage;

char *givePresetMessage(DracView gameState, DraculaMessage code);

void decideDraculaMove(DracView gameState) {

	Round roundNum = giveMeTheRound(gameState);
	//LocationID GLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
	//LocationID SLocation = whereIs(gameState, PLAYER_DR_SEWARD);
	//LocationID HLocation = whereIs(gameState, PLAYER_VAN_HELSING);
	//LocationID MLocation = whereIs(gameState, PLAYER_MINA_HARKER);
	LocationID DLocation = whereIs(gameState, PLAYER_DRACULA);
	if (roundNum == 0) {
		registerBestPlay("FR", givePresetMessage(gameState, 6));
	} else {
        // Creating an empty set which will be filled with all legal moves by Dracula in current state
        Set options = newSet();
        // connections[] contains all locations Dracula can reach in one round minus the ones currently in his trail
		int numConnections = 0;
		LocationID *connections = whereCanIgo(gameState, &numConnections, TRUE, TRUE);
        int numOptions = numConnections;
        int i;
        for (i = 0; i < numConnections; i++) {
            setAdd(options, connections[i]);
        }
        if (doneHide(gameState) == FALSE && idToType(DLocation) != SEA) {
            setAdd(options, HIDE);
            numOptions++;
        }
        if (doneDoubleBack(gameState) == FALSE) {
            setAdd(options, DOUBLE_BACK_1);
            numOptions++;
            if (roundNum >= 2) {
                setAdd(options, DOUBLE_BACK_2);
                numOptions++;
            }
            if (roundNum >= 3) {
                setAdd(options, DOUBLE_BACK_3);
                numOptions++;
            }
            if (roundNum >= 4) {
                setAdd(options, DOUBLE_BACK_4);
                numOptions++;
            }
            if (roundNum >= 5) {
                setAdd(options, DOUBLE_BACK_5);
                numOptions++;
            }
        }
        if (numOptions == 0) {
            setAdd(options, TELEPORT);
            numOptions++;
        }
        LocationID * optionsArray = copySetToArray(options);
        srand(time(NULL));
		int choiceIndex = rand() % numOptions;
		int messageCode = rand() % 5;
        char * choice;
        if (optionsArray[choiceIndex] >= MIN_MAP_LOCATION  && optionsArray[choiceIndex] <= MAX_MAP_LOCATION) {
            choice = idToAbbrev(optionsArray[choiceIndex]);
        } else if (optionsArray[choiceIndex] == HIDE) {
            choice = "HI";
        } else if (optionsArray[choiceIndex] == DOUBLE_BACK_1) {
            choice = "D1";
        } else if (optionsArray[choiceIndex] == DOUBLE_BACK_2) {
            choice = "D2";
        } else if (optionsArray[choiceIndex] == DOUBLE_BACK_3) {
            choice = "D3";
        } else if (optionsArray[choiceIndex] == DOUBLE_BACK_4) {
            choice = "D4";
        } else if (optionsArray[choiceIndex] == DOUBLE_BACK_5) {
            choice = "D5";
        } else if (optionsArray[choiceIndex] == TELEPORT) {
            choice = "TP";
        }
	registerBestPlay(choice, givePresetMessage(gameState, messageCode));
        free(optionsArray);
        disposeSet(options);
	free(connections);
    }
}



// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
char *givePresetMessage(DracView gameState, DraculaMessage code) {
	switch (code) {
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
		default:
			return "Mwuhahahaha";
	}
}
