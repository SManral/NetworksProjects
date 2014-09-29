#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

Queue *create_queue(int max_cells) {
  Queue *new_queue; // Holds pointer to the newly-allocated Queue structure.
  new_queue = (Queue *) malloc(sizeof(Queue)); //allocate memeory for structure

  if (new_queue == NULL) return NULL; // Error--unable to allocate.

  // Fill in the struct
  new_queue->max_cells = max_cells;
  new_queue->cells_used = 0; // Empty to start
  
  // Now allocate space for the queue entries.
  new_queue->queue_base = (void **) calloc(sizeof(void *), max_cells);
  if (new_queue->queue_base == NULL) {
    free(new_queue); // Unable to allocate queue entries, so free struct.
    return NULL;
  }
  new_queue->tail = new_queue->queue_base; // Start at base
  new_queue->head = new_queue->queue_base; // Put head at base as well
  
  //set the end field of the queue to a pointer to the last cell
  void **queue_stepthrough = new_queue->queue_base;
    queue_stepthrough += max_cells;
  new_queue->end = queue_stepthrough;    

  return new_queue; //return a pointer to the newly created queue
}

int enqueue(Queue *which_queue, void *ptr) {
  // Check if Queue is already full 
  if ((which_queue->cells_used) >= (which_queue->max_cells)) { 
    which_queue->cells_used = which_queue->max_cells;
    printf("Attempted to queue element number %d which is not permitted\n", which_queue->max_cells + 1); 
    return -1;  // Queue overflow.
  }
   
  // Put object into Queue, 
  *(which_queue->tail) = ptr;  // Store the pointer on the queue
  (which_queue->cells_used)++; //Increase number of cells used
  if (which_queue->end == which_queue->tail) 
    which_queue->tail = which_queue->queue_base; //wraparound if necessary
  else
    (which_queue->tail)++;// Have tail pointer point to next free cell if wraparound isn't needed. Tail pointer can point to the same location as head
  return 0;  // Success
}

void* dequeue(Queue *which_queue) {
  // Check if queue is empty
  if ((which_queue->cells_used) < 1) { //changed to one
    which_queue->cells_used = 0; 
    printf("Attempted to remove an item on an empty queue.\n");
    return NULL;  // queue empty
  }
  
  (which_queue->cells_used)--;
  // Remove element from queue
  if (which_queue->head == which_queue->end) {
    which_queue->head = which_queue->queue_base;
    return *(which_queue->end);
  }
  else { 
    (which_queue->head)++;  // This now points to the new begining of the queue
  return (*(which_queue->head - 1));
  }
}

void* peek(Queue *which_queue) {
  // Check if queue is empty
  if ((which_queue->cells_used) < 1) { //will not allow peek to be called on an empty queue
    which_queue->cells_used = 0; 
    printf("Attempted to peek at an empty queue.\n"); 
    return NULL;  // Queue empty
  }

  // Return top of queue, without dequeueing.
  void** top = (which_queue->head); //point to top of queue

  return (*top); // Get the entry
}

Bar* queueBar(int bar_value, Queue *new_queue) {
  Bar* new_bar; //initializes a pointer to a bar struct
  new_bar = (Bar *) malloc(sizeof(Bar)); //allocates memeory for the struct
  new_bar->x = bar_value; //sets the new struct's value
  printf("Queueing: x = %5d.\n", new_bar->x); //prints the new struct
  enqueue(new_queue, (void *) new_bar); //queues the new struct into the passed in queue
  return new_bar; //returns the bar
}
