#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int main() {

  const int max_entries = 3; // size of Queue
  Bar* new_bar1;
  Bar* new_bar2; 
  Bar* new_bar3;
  Bar* new_bar4;
  Bar* new_bar5;
  Bar* new_bar6;
  Bar* new_bar7;
  Bar* returned_bar;

  // First, create a Queue
  Queue *new_queue = create_queue(max_entries);

  // Allocate 2 bars and put them into the queue
  new_bar1 = queueBar(1, new_queue);
  new_bar2 = queueBar(2, new_queue);

  // Peek at the top of the queue
  returned_bar = (Bar *) peek(new_queue);
  printf("Peeked:  x = %5d\n", returned_bar->x);

  //Dequeue 2 elements, displaying both, then attempt to dequeue a third
  returned_bar = (Bar *) dequeue(new_queue);
  printf("Dequeued:  x = %5d\n", returned_bar->x);
  returned_bar = (Bar *) dequeue(new_queue);
  printf("Dequeued:  x = %5d\n", returned_bar->x);
  returned_bar = (Bar *) dequeue(new_queue);

  // Allocate and queue two elements to check for proper wraparound
  new_bar3 = queueBar(3, new_queue);
  new_bar4 = queueBar(4, new_queue);

  // Allocate and queue one more element, then attempt to allocate and queue a fourth, checking for proper queue overflow checking
  new_bar5 = queueBar(5, new_queue);
  new_bar6 = queueBar(6, new_queue);
  
  //frees the unused overflow item
  free(new_bar6);

  // Peeks, dequeues two elements, peeks, allocates and adds one, then peeks again to make sure the functions are working fully as intended
  returned_bar = (Bar *) peek(new_queue);
  printf("Peeked:  x = %5d\n", returned_bar->x);
  returned_bar = (Bar *) dequeue(new_queue);
  printf("Dequeued:  x = %5d\n", returned_bar->x);
  returned_bar = (Bar *) dequeue(new_queue);
  printf("Dequeued:  x = %5d\n", returned_bar->x);
  returned_bar = (Bar *) peek(new_queue);
  printf("Peeked:  x = %5d\n", returned_bar->x);
  new_bar6 = queueBar(6, new_queue);
  returned_bar = (Bar *) peek(new_queue);
  printf("Peeked:  x = %5d\n", returned_bar->x);
  
  return 0; //success!
}
