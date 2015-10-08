// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "HunterView.h"


void decideHunterMove(HunterView gameState)
{
    int turnNo = giveMeTheRound(gameState);
    printf("round:%d\n", turnNo); 
    printf("ENTRY\n");
    if(turnNo <= 1){
       printf("1\n");
       registerBestPlay("CD","Let's begin at Castle Dracula");
    }else{
       printf("2\n");
       srand(time(NULL));
       printf("2.5\n");
       int amountMoves;
       LocationID * moves = whereCanIgo(gameState, &amountMoves, TRUE, FALSE,TRUE);
       printf("moves:%d\n", amountMoves);
       LocationID decision = rand() % amountMoves;
       char * choice = idToAbbrev(moves[decision]); 
       registerBestPlay(choice,"I don't think he's showing up...");
       free(moves);
    }
    printf("3\n");
    printf("END\n");
}

