#ifndef QUEUE_H
#define QUEUE_H

/** Struct to define a Queue */
typedef struct queue {
  void **queue_base; // Pointer to base of queue
  void **tail;  // Pointer to next free cell;
  void **head; // Pointer to least recently added cell
  int max_cells; // Maximum number of entries in the queue
  int cells_used; // Currently used number of cells
  void **end; //Pointer to the end of the queue
} Queue;

typedef struct {
  int x;
} Bar; // Just some arbitrary struct

 //Function prototypes

/** Creates a queue by allocating a queue structure, and setting the tail and head pointers to the begining of the newly-allocated struct in memory, and then initializes it. Space is then allocated for max_cells elements
 * @param max_cells maximum number of cells this queue can hold
 * @return returns a pointer to the newly-allocated structure, or null if error */
Queue *create_queue(int max_cells);

/** Adds an element to the tail of a queue, moving foward the tail pointer to the next cell. If queue is full, the tail will loop back to the begining instead of an empty block of memory. 
 * @param which_queue pointer to the queue to be added to
 * @param ptr Pointer to the object to be added to the queue
 * @return 0 if success, -1 if failure */ 
int enqueue(Queue *which_queue, void *ptr);

/** Removes the element which head is pointing to, the least recently added element, then points to the next element. If the last element in the queue is removed, the head pointer loops to the first element. 
 * @param which_queue pointer to the queue you want to remove an element from
 * @return Returns a pointer to the removed element */
void* dequeue(Queue *which_queue);

/** Returns the first element in the queue, without removing it
 * @param which_queue Queue to be peeked at
 * @return Top entry of the queue, null if empty */
void* peek(Queue *which_queue);

/** Creates a bar with the field equal to the parameter bar_value, then puts the bar into the given queue
 * @param bar_value field of the bar to be generated
 * @param new_queue Queue to have the bar placed in it
 * @return Returns a pointer to the bar created, null if empty */
Bar* queueBar(int bar_value, Queue *new_queue);

#endif
