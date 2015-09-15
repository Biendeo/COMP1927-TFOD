// Queue.c
// A queue implementation used for TFOD.
// This is specifically for the trail.

#include <stdlib.h>
#include "Places.h"
#include "Queue.h"

void slide(Trail trail, int index);
LocationID pop(Trail trail);

// The queue is basically a filled list.
struct Trail {
	LocationID * trail;
	int size;
};

// Makes a new trail of a given size
Trail newTrail (int size) {
	Trail newTrail = malloc(sizeof(Trail));
	newTrail->trail = calloc(size, sizeof(LocationID));
	newTrail->size = size;
	// This just flushes the trail with null values.
	for (int i = 0; i < size; i++) {
		newTrail->trail[i] = NULL_TERM;
	}
	return newTrail;
}

// Deletes a trail.
void disposeTrail (Trail trail) {
	free(trail->trail);
	free(trail);
}

// This adds an element to the beginning of the queue.
void prepend (Trail trail, LocationID place) {
	for (int i = trail->size; i > 0; i--) {
		slide(trail, i);
	}
	trail->trail[0] = place;
}

// This returns the element of a given index.
LocationID showElement (Trail trail, int index) {
	if (index >= trail->size || index < 0) {
		return NULL_TERM;
	} else {
		return trail->trail[index];
	}
}


// ---
// EVERYTHING FROM HERE ON OUT IS NOT A PUBLIC FUNCTION.

// This slides an element one further down the list.
// If it exceeds the size, it pops off.
void slide(Trail trail, int index) {
	if (index == trail->size - 1) {
		pop(trail);
	} else {
		trail->trail[index + 1] = trail->trail[index];
		trail->trail[index] = NULL_TERM;
	}
}

// This removes the last element and returns it.
LocationID pop(Trail trail) {
	LocationID returnLocation = trail->trail[trail->size - 1];
	trail->trail[trail->size - 1] = NULL_TERM;
	return returnLocation;
}