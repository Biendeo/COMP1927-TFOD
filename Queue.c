// Queue.c
// A queue implementation used for TFOD.
// This is specifically for the trail.

#include <stdlib.h>
#include "Places.h"
#include "Queue.h"

// Trail code

void slide(Trail trail, int index);
LocationID pop(Trail trail);

// The queue is basically a filled list.
struct Trail {
	LocationID *trail;
	int size;
};

// Makes a new trail of a given size
Trail newTrail(int size) {
	Trail newTrail = malloc(sizeof(struct Trail));
	newTrail->trail = malloc(sizeof(LocationID) * size);
	newTrail->size = size;
	// This just flushes the trail with null values.
	for (int i = 0; i < size; i++) {
		newTrail->trail[i] = UNKNOWN_LOCATION;
	}
	return newTrail;
}

// Deletes a trail.
void disposeTrail(Trail trail) {
	free(trail->trail);
	free(trail);
}

// This adds an element to the beginning of the queue.
void prepend(Trail trail, LocationID place) {
	for (int i = trail->size - 1; i >= 0; i--) {
		slide(trail, i);
	}
	trail->trail[0] = place;
}

// This returns the element of a given index.
LocationID showElement(Trail trail, int index) {
	if (index >= trail->size || index < 0) {
		return UNKNOWN_LOCATION;
	} else {
		return trail->trail[index];
	}
}

// This removes the oldest element containing a given value 
void removeElement(Trail trail, LocationID place) {
	int found = 0;
	int i;
	for (i = trail->size - 1; found != 1 && i >= 0; i--) {
		if (showElement(trail, i) == place) {
			found = 1;
		}
	}
	if (found == 0) {
		return; // Element not found, return early
	} else {
		int elementIndex = i;
		if (i == 0) { // If found element is already at the start of the list, can't slide
			trail->trail[0] = UNKNOWN_LOCATION;
		} else {
			for (i = elementIndex; i >= 0; i--) {
				slide(trail, i - 1);
			}
		}
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
		trail->trail[index] = UNKNOWN_LOCATION;
	}
}

// This removes the last element and returns it.
LocationID pop(Trail trail) {
	LocationID returnLocation = trail->trail[trail->size - 1];
	trail->trail[trail->size - 1] = UNKNOWN_LOCATION;
	return returnLocation;
}


// Queue code

typedef struct QueueNode *QueueNode;

struct QueueNode {
	QueueNode next;
	LocationID value;
};

struct Queue {
	QueueNode first;
	QueueNode last;
	int size;
};

QueueNode newQueueNode(LocationID value);

// Creates a new queue.
Queue newQueue() {
	Queue queue = malloc(sizeof(struct Queue));
	queue->first = NULL;
	queue->last = NULL;
	queue->size = 0;

	return queue;
}

// Deletes a queue.
void disposeQueue(Queue queue) {
	while (queue->first != NULL) {
		queuePop(queue);
	}
	free(queue);
}

// Adds an element to the end of the queue.
void queueAdd(Queue queue, LocationID value) {
	QueueNode node = newQueueNode(value);
	if (queue->size == 0) {
		queue->last = node;
		queue->first = node;
	} else {
		queue->last->next = node;
		queue->last = node;
	}
	queue->size++;
}

// Removes the front element from the queue.
LocationID queuePop(Queue queue) {
	QueueNode popNode = queue->first;
	if (popNode == NULL) {
		return -1;
	}
	LocationID popValue = queue->first->value;
	queue->first = queue->first->next;
	free(popNode);
	return popValue;
}

// Returns the size of the queue.
int getQueueSize(Queue queue) {
	return queue->size;
}

// ---
// EVERYTHING FROM HERE ON OUT IS NOT A PUBLIC FUNCTION.

// Creates a new queue node.
QueueNode newQueueNode(LocationID value) {
	QueueNode node = malloc(sizeof(struct QueueNode));
	node->value = value;
	node->next = NULL;
	return node;
}