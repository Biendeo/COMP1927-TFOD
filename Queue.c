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

//QUEUE FOR GRAPH TRAVERSAL
typedef struct Node *Link;
typedef struct Node {
	LocationID val;
	Link  next;
} Node;
	
typedef struct QueueRep {
	Link  front;
	Link  back;
} QueueRep;

// Function signatures

Queue newQueue();
void disposeQueue(Queue);
void enterQueue(Queue,LocationID);
LocationID leaveQueue(Queue);
int  emptyQueue(Queue);
void showQueue(Queue q);

static Link newNode(LocationID);
static void disposeNode(Link);

// Makes a new trail of a given size
Trail newTrail (int size) {
	Trail newTrail = malloc(sizeof(struct Trail));
	newTrail->trail = calloc(size, sizeof(LocationID));
	newTrail->size = size;
	// This just flushes the trail with null values.
	for (int i = 0; i < size; i++) {
		newTrail->trail[i] = UNKNOWN_LOCATION;
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
		return UNKNOWN_LOCATION;
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
		trail->trail[index] = UNKNOWN_LOCATION;
	}
}

// This removes the last element and returns it.
LocationID pop(Trail trail) {
	LocationID returnLocation = trail->trail[trail->size - 1];
	trail->trail[trail->size - 1] = UNKNOWN_LOCATION;
	return returnLocation;
}



//GRAPH TRAVERSAL FUNCTIONS
// newQueue()
// - create an initially empty Queue
Queue newQueue()
{
	Queue new = malloc(sizeof(QueueRep));
	assert(new != NULL);
	new->front = NULL;
	new->back = NULL;
	return new;
}

// disposeQueue(Queue)
// - clean up memory associated with Queue
void disposeQueue(Queue q)
{
	if (q == NULL) return;
	Link next, curr = q->front;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
}

// enterQueue(Queue,Str)
// - add Src to back of Queue
void enterQueue(Queue q, LocationID src)
{
	Link new = newNode(src);
	if (q->front == NULL)
		q->front = q->back = new;
	else {
		// add into list of elems
		q->back->next = new;
		q->back = new;
	}
}

// leaveQueue(Queue)
// - return string at front of Queue
LocationID leaveQueue(Queue q)
{
	assert (q->front != NULL);
    LocationID src = q->front->val;
	Link old = q->front;
	q->front = old->next;
	if (q->front == NULL)
		q->back = NULL;
	free(old);
	return src;
}

// emptyQueue(Queue)
// - check whether Queue is empty
int emptyQueue(Queue q)
{
	return (q->front == NULL);
}

// showQueue(Queue)
// - display Queue (for debugging)
void showQueue(Queue q)
{
	Link curr;
	if (q->front == NULL)
		printf("Queue is empty\n");
	else {
		printf("Queue (front-to-back):\n");
		int id = 0;
		curr = q->front;
		while (curr != NULL) {
			printf("%d\n", curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode(LocationID src)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = src;
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr->val);
	free(curr);
}