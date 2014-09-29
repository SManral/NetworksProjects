#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"
#define NAK (2)
#define ACK (1)
#define DATAACK (999)
#define TIMERAMT 1000
 
/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for Project 3, unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order.

   Compile as gcc -g project3.c student3.c -o p3
**********************************************************************/



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/* 
 * The routines you will write are detailed below. As noted above, 
 * such procedures in real-life would be part of the operating system, 
 * and would be called by other procedures in the operating system.  
 * All these routines are in layer 4.
 */

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
int corrupted = 0;
int Bstate = 0x0;
int Astate = 0x0;
struct pkt outputbuff;
Queue* globalqueue;
struct pkt* outputbuffptr;
struct pkt* temp;

void A_output(struct msg message) {
  //pktqueuetest();
 // pktqueuetest2();
  temp = (struct pkt*) malloc(sizeof(struct pkt));
  memcpy(temp->payload,message.data,MESSAGE_LENGTH);
  temp->seqnum = Astate;
  Astate = Astate ^ 0x1;
  temp->acknum = DATAACK;
  temp->checksum = 0;
  temp->checksum = genchecksum(*temp);
  if (!getTimerStatus( AEntity )) { //will succeed if timer is off
    outputbuff = *temp;
    outputbuffptr = temp;
    tolayer3(AEntity,*temp);
    startTimer(AEntity,TIMERAMT);
    return;
  }
  else {
    enqueue(globalqueue, (void *) temp);
    //printf("queued packet with data %.*s\n",20,message.data);
    struct pkt* ptemp = (struct pkt*) peek(globalqueue);
    //printf("peeking at queue, first element has string %s\n",ptemp->payload);
    return;
  }
}

/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */
void B_output(struct msg message)  {

}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
  stopTimer( AEntity );
  //struct pkt* ptemp = (struct pkt*) peek(globalqueue);
  //printf("peeking at queue, first element has string %s\n",ptemp->payload);
  int tempack = packet.acknum;
  //packet.acknum = DATAACK;
  struct pkt* next;
  if(iscorrupt(packet)) {
    tolayer3(AEntity,outputbuff);
    startTimer(AEntity,TIMERAMT);
    return;
  }
  //packet.acknum = tempack;
  if (packet.acknum == NAK) {
    tolayer3(AEntity,outputbuff);
    startTimer(AEntity,TIMERAMT);
  }
  if (packet.acknum == ACK) {
    //free(outputbuffptr);
    //Astate = Astate ^ 0x1; //flip state
    if (peek(globalqueue) == NULL) {
      return;
    }
    next = (struct pkt*) dequeue(globalqueue);
   // printf("dequeued packet with data %s\n",next->payload);
    outputbuff = *next;
    outputbuffptr = next;
    tolayer3(AEntity,outputbuff); //should be *next
    startTimer(AEntity,TIMERAMT);
  }  
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
  tolayer3(AEntity,outputbuff);
  startTimer(AEntity, TIMERAMT);
  return;
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  stopTimer(AEntity);
  globalqueue = create_queue(MAX_CELLS);
}


/* 
 * Note that with simplex transfer from A-to-B, there is no routine  B_output() 
 */

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
  struct pkt ack;
  struct pkt nak;
  if (iscorrupt(packet)) {
    nak.checksum = 0;
    nak.seqnum = packet.seqnum;
    nak.acknum = NAK;
    memcpy(nak.payload,packet.payload,MESSAGE_LENGTH);
    nak.checksum = genchecksum(nak);
    tolayer3(BEntity,nak);
    return;
  }
  
   if (!iscorrupt(packet) && packet.seqnum != Bstate) { //wrong seq num
    ack.checksum = 0;
    ack.seqnum = packet.seqnum;
    ack.acknum = ACK;
    memcpy(ack.payload,packet.payload,MESSAGE_LENGTH);
  //  printf("resending old ack\n");
    ack.checksum = genchecksum(ack);
    tolayer3(BEntity,ack); 
    return;
  }
  if (!iscorrupt(packet) && packet.seqnum == Bstate) { //right seqnum
    ack.checksum = 0;
    ack.seqnum = packet.seqnum;
    ack.acknum = ACK;
    memcpy(ack.payload,packet.payload,MESSAGE_LENGTH);
    ack.checksum = genchecksum(ack);
    tolayer3(BEntity,ack); 
    struct msg message;
    memcpy(message.data,packet.payload,MESSAGE_LENGTH); 
    tolayer5(BEntity,message);
    Bstate = Bstate ^ 0x1;
    return; 
  }/*
  struct msg msg;
  memcpy(msg.data,packet.payload,MESSAGE_LENGTH);  
  tolayer5(BEntity,msg);*/
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
}

/*
 * The following functions generates a checksum for the packet by using fletchers algorithms on the binary data of the structure, using blocks of 2 bytes*/
int iscorrupt(struct pkt packet) {
  int checksum1 = packet.checksum;
  packet.checksum = 0;
  int checksum2 = genchecksum(packet);
  //printf("checksum1: %d checksum2: %d\n",checksum1,checksum2);
  if (checksum1 != checksum2) {
    //printf("Packet is corrupted\n");
    corrupted++;
    //printf("Total packets corrupted %d\n",corrupted);
    return TRUE;
  }
  packet.checksum = checksum1;
  return FALSE;
}
  

int genchecksum(struct pkt packet) {
  packet.checksum = 0;
  unsigned short* iterator = (short*) &packet;
  int size = (sizeof(struct pkt) / sizeof(short));
  unsigned int checksum1 = 0;
  unsigned int checksum2 = 0;
  unsigned int csfinal = 0;
  int i = 0;
  for (i = 0; i < size; i++,iterator++) {
    checksum1 = ((checksum1 + *iterator) % 65535);
    checksum2 = ((checksum1 + checksum2) % 65535);
  }
  csfinal = checksum2;
  csfinal = (csfinal << 16) | checksum1;
  //csfinal = 0x7FFFFFFF & csfinal;
  return csfinal;
}

/**********************************************************/
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

void queuepkt(struct pkt packet, Queue *new_queue) {
  enqueue(new_queue, (void *) &packet);
}
 
void pktqueuetest() {
  struct pkt p;
  p.seqnum = 1;
  p.acknum = 0;
  p.checksum = 100;
  enqueue(globalqueue, (void *) &p);
  return;
}

void pktqueuetest2() {
 /* printf("peeking at queue\n");
  struct pkt* peek1 = (struct pkt*) peek(globalqueue);
  printf("peeked packet's checksum %d\n",peek1->checksum);
  struct pkt peek2 = *peek1;
  printf("printed from struct checksum of peeked: %d\n",peek2.checksum);
  struct pkt p2;
  p2.checksum = 200;
  enqueue(globalqueue, (void*) &p2);
  dequeue(globalqueue); */
  struct pkt* pkt3 = (struct pkt*) dequeue(globalqueue);
  struct pkt pkt = *pkt3;
  printf("pkt3 checksum expecting 100: %d\n",pkt.checksum);
  exit(0);
  return;
}
