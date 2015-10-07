// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

/*TO-DO List
1)Start all hunters at Castle Dracula.  This should get us a few instakills
  on the teams that didn't write a dracula AI.
2)Implement rest (use when health lower than half)
3)Implement shortest path searches

  Strategy 1: A hunter is selected as a leader. All other hunters must stay within
  1 edge between themselves and the leader, trying as much as possible to not be
  in the same city as another hunter. When a hunter discovers Dracula's trail,
  they become the leader and must pursue Dracula. When a hunter finds dracula, all
  other hunters converge on dracula.

  Pros:-Once we find Dracula, there is a pretty good chance we will win instantly
       as all hunters will hit dracula for damage equivalent to his starting blood
  Cons:-Hard to find Dracula due to compactness of hunters
       -If a hunter dies, they can get split up from the cluster and the algorithm
        may waste time trying to regroup the hunters.

*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
//#include "Places.h"
#include "HunterView.h"

char *givePresetMessage(HunterView gameState);

void decideHunterMove(HunterView gameState){
/*
        srand(time(NULL));

        int turnNo = giveMeTheRound(gameState);
        //int curHunter = whoAmI(gameState);

        if(turnNo <= 1){

	   registerBestPlay("CD", givePresetMessage(gameState));

        } else if(turnNo % 2 == 0) {

           int * amountOfMoves = 0;//Just in case whereCanIGo does stuff with the "initial value"
           LocationID *moves = whereCanIgo(gameState,amountOfMoves,TRUE,FALSE,TRUE);

           //int moveChoice = rand() % *amountOfMoves;
           char * choice = idToAbbrev(moves[0]);

           free(moves);

           registerBestPlay(choice, givePresetMessage(gameState));

        }else{
*/
            registerBestPlay("CD", givePresetMessage(gameState));

  //      }

}

// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
char *givePresetMessage(HunterView gameState) {
	switch (giveMeTheRound(gameState)) {
		default:
			return "Hey Guys, lets chill out at Castle Dracula!\n";
	}
}
