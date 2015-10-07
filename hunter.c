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

/*


typedef struct Place {
   char      *name;
   char      *abbrev;
   LocationID id;
   PlaceType  type;
} Place;

// Places should appear in alphabetic order
// Each entry should satisfy (places[i].id == i)
// First real place must be at index MIN_MAP_LOCATION
// Last real place must be at index MAX_MAP_LOCATION
static Place places[] =
{
   {"Adriatic Sea", "AS", ADRIATIC_SEA, SEA},
   {"Alicante", "AL", ALICANTE, LAND},
   {"Amsterdam", "AM", AMSTERDAM, LAND},
   {"Athens", "AT", ATHENS, LAND},
   {"Atlantic Ocean", "AO", ATLANTIC_OCEAN, SEA},
   {"Barcelona", "BA", BARCELONA, LAND},
   {"Bari", "BI", BARI, LAND},
   {"Bay of Biscay", "BB", BAY_OF_BISCAY, SEA},
   {"Belgrade", "BE", BELGRADE, LAND},
   {"Berlin", "BR", BERLIN, LAND},
   {"Black Sea", "BS", BLACK_SEA, SEA},
   {"Bordeaux", "BO", BORDEAUX, LAND},
   {"Brussels", "BU", BRUSSELS, LAND},
   {"Bucharest", "BC", BUCHAREST, LAND},
   {"Budapest", "BD", BUDAPEST, LAND},
   {"Cadiz", "CA", CADIZ, LAND},
   {"Cagliari", "CG", CAGLIARI, LAND},
   {"Castle Dracula", "CD", CASTLE_DRACULA, LAND},
   {"Clermont-Ferrand", "CF", CLERMONT_FERRAND, LAND},
   {"Cologne", "CO", COLOGNE, LAND},
   {"Constanta", "CN", CONSTANTA, LAND},
   {"Dublin", "DU", DUBLIN, LAND},
   {"Edinburgh", "ED", EDINBURGH, LAND},
   {"English Channel", "EC", ENGLISH_CHANNEL, SEA},
   {"Florence", "FL", FLORENCE, LAND},
   {"Frankfurt", "FR", FRANKFURT, LAND},
   {"Galatz", "GA", GALATZ, LAND},
   {"Galway", "GW", GALWAY, LAND},
   {"Geneva", "GE", GENEVA, LAND},
   {"Genoa", "GO", GENOA, LAND},
   {"Granada", "GR", GRANADA, LAND},
   {"Hamburg", "HA", HAMBURG, LAND},
   {"Ionian Sea", "IO", IONIAN_SEA, SEA},
   {"Irish Sea", "IR", IRISH_SEA, SEA},
   {"Klausenburg", "KL", KLAUSENBURG, LAND},
   {"Le Havre", "LE", LE_HAVRE, LAND},
   {"Leipzig", "LI", LEIPZIG, LAND},
   {"Lisbon", "LS", LISBON, LAND},
   {"Liverpool", "LV", LIVERPOOL, LAND},
   {"London", "LO", LONDON, LAND},
   {"Madrid", "MA", MADRID, LAND},
   {"Manchester", "MN", MANCHESTER, LAND},
   {"Marseilles", "MR", MARSEILLES, LAND},
   {"Mediterranean Sea", "MS", MEDITERRANEAN_SEA, SEA},
   {"Milan", "MI", MILAN, LAND},
   {"Munich", "MU", MUNICH, LAND},
   {"Nantes", "NA", NANTES, LAND},
   {"Naples", "NP", NAPLES, LAND},
   {"North Sea", "NS", NORTH_SEA, SEA},
   {"Nuremburg", "NU", NUREMBURG, LAND},
   {"Paris", "PA", PARIS, LAND},
   {"Plymouth", "PL", PLYMOUTH, LAND},
   {"Prague", "PR", PRAGUE, LAND},
   {"Rome", "RO", ROME, LAND},
   {"Salonica", "SA", SALONICA, LAND},
   {"Santander", "SN", SANTANDER, LAND},
   {"Saragossa", "SR", SARAGOSSA, LAND},
   {"Sarajevo", "SJ", SARAJEVO, LAND},
   {"Sofia", "SO", SOFIA, LAND},
   {"St Joseph and St Marys", "JM", ST_JOSEPH_AND_ST_MARYS, LAND},
   {"Strasbourg", "ST", STRASBOURG, LAND},
   {"Swansea", "SW", SWANSEA, LAND},
   {"Szeged", "SZ", SZEGED, LAND},
   {"Toulouse", "TO", TOULOUSE, LAND},
   {"Tyrrhenian Sea", "TS", TYRRHENIAN_SEA, SEA},
   {"Valona", "VA", VALONA, LAND},
   {"Varna", "VR", VARNA, LAND},
   {"Venice", "VE", VENICE, LAND},
   {"Vienna", "VI", VIENNA, LAND},
   {"Zagreb", "ZA", ZAGREB, LAND},
   {"Zurich", "ZU", ZURICH, LAND},
};

char * LocNumToAbbrev(LocationID p){

   return places[p].abbrev;

}

*/

void decideHunterMove(HunterView gameState){

		/*srand(time(NULL));

		int turnNo = giveMeTheRound(gameState);
		//int curHunter = whoAmI(gameState);

		if(turnNo <= 1){

	   registerBestPlay("CD", givePresetMessage(gameState));

		} else if(turnNo % 2 == 0) {

		   int * amountOfMoves = 0;//Just in case whereCanIGo does stuff with the "initial value"
		   LocationID *moves = whereCanIgo(gameState,amountOfMoves,TRUE,FALSE,TRUE);
		   int moveChoice = rand() % *amountOfMoves;
		   char * choice = idToAbbrev(moves[moveChoice]);

		   registerBestPlay(choice, givePresetMessage(gameState));
		   free(moves);

		}else{*/

			registerBestPlay("CD", givePresetMessage(gameState));

}

// Returns a witty message depending on game features.
// As Dracula, this is pretty useless, but it'll be funny to read later.
char *givePresetMessage(HunterView gameState) {
	switch (giveMeTheRound(gameState)) {
		default:
			return "Hey Guys, lets chill out at Castle Dracula!\n";
	}
}
