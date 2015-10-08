// Set.c
// A set implementation used for TFOD.
// This is specifically for the the graph.

#include <stdlib.h>
#include "Places.h"
#include "Set.h"

typedef struct SetNode *SetNode;

struct SetNode {
	LocationID value;
	SetNode next;
};

// The set is basically a linked list.
struct Set {
	SetNode start;
	int size;
};

SetNode getLast(Set set);

// Makes a new set
Set newSet () {
	Set newSet = malloc(sizeof(struct Set));
	newSet->size = 0;
	newSet->start = NULL;
	return newSet;
}

// Deletes a set.
void disposeSet (Set set) {
	SetNode node = set->start;
	SetNode nextNode;
	while (node != NULL) {
		nextNode = node->next;
		free(node);
		node = nextNode;
	}
	free(set);
}

// Adds an element to the set, unless it already is there.
void setAdd(Set set, LocationID place) {
	if (!isElem(set, place)) {
		SetNode end = getLast(set);
		if (end == NULL) {
			set->start = malloc(sizeof(struct SetNode));
			set->start->value = place;
			set->start->next = NULL;
		} else {
			end->next = malloc(sizeof(struct SetNode));
			end->next->value = place;
			end->next->next = NULL;
		}
		set->size++;
	}
}

// Removes an element from the set, unless it's not there.
void setRemove(Set set, LocationID place) {
	if (isElem(set, place)) {
		SetNode previousNode = set->start;
		SetNode currentNode = set->start;
		while (currentNode->value != place) {
			if (currentNode != set->start) {
				previousNode = currentNode;
			}
			currentNode = currentNode->next;
		}
		SetNode nextNode = currentNode->next;
		if (currentNode == set->start) {
			set->start = nextNode;
		} else {
			previousNode->next = nextNode;
		}
		free(currentNode);
		set->size--;
	}
}

// Returns 1 if the element is in the set, or 0 if it isn't.
int isElem(Set set, LocationID place) {
	for (SetNode node = set->start; node != NULL; node = node->next) {
		if (node->value == place) return 1;
	}
	return 0;
}

// Creates an array with all the elements in the set.
LocationID *copySetToArray(Set set) {
	int size = 0;
	for (SetNode node = set->start; node != NULL; node = node->next) {
		size++;
	} // could call getSetSize(set) here?
	LocationID *arr = malloc(sizeof(LocationID) * size);
	int i = 0;
	for (SetNode node = set->start; node != NULL; node = node->next) {
		arr[i] = node->value;
		i++;
	}
	return arr;
}

// Returns the size.
int getSetSize(Set set) {
	return set->size;
}

// Creates a set with all the elements of the array (but with no duplicates).
Set copyArrayToSet(LocationID *arr, int size) {
	Set set = newSet();
	for (int i = 0; i < size; i++) {
		setAdd(set, arr[i]);
	}
	return set;
}

// ---
// EVERYTHING FROM HERE ON OUT IS NOT A PUBLIC FUNCTION.

// Gets the last node in the set.
SetNode getLast(Set set) {
	SetNode node = set->start;
	if (set->size == 0) {
		return NULL;
	} else if (set->size == 1) {
		return node;
	} else {
		while (node->next != NULL) {
			node = node->next;
		}
		return node;
	}
}
