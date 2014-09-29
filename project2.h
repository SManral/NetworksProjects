/*
 * This is the include file that contains definitions of various structures.
 * It also contains prototypes needed by the routines in the student code.
 */

/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: 

   This code should be used for Project 3, unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order
 VERSION 1.1  J.F.Kurose
 VERSION 2.0  J. Breecher  - October  2005
 VERSION 2.05 J. Breecher  - November 2005
 VERSION 2.10 J. Breecher  - February 2011
 VERSION 2.20 J. Breecher  - October  2013
*****************************************************************************/

#define   LINUX
//#define    WINDOWS

#define  TRUE            1
#define  FALSE           0
/*  This defines which side (entity) is making a call to the simulator */
#define  AEntity         0
#define  BEntity         1

// A "msg" is the data unit passed from layer 5 (Application code) to layer 
// 4 (student's code).  It contains the data (characters) to be delivered 
// to the remote layer 5 via the students transport level protocol entities.  

#define  MESSAGE_LENGTH  20
#define MAX_CELLS 110
struct msg {
    char  data[MESSAGE_LENGTH];
};

// A packet is the data unit passed from layer 4 (student's Transport Layer
// code) to layer 3 (the network simulator).  Note this is the pre-defined
// packet structure, that is expected by layer 3 - so student code must 
// match this format.        
struct   pkt {
    int  seqnum;
    int  acknum;
    int  checksum;
    char payload[MESSAGE_LENGTH];
};

/*
 * PROTOTYPES - These are in student.c
 * */

void A_output(struct msg message);  // A-Side App call to A-side transport
void B_output(struct msg message);  // B-Side App call to B-side transport
void A_input(struct pkt packet);    // From network layer UP to A transport
void A_timerinterrupt();            // A-side code that is called on interrupt
void A_init();                      // Simulator calls this at initialization
void B_input(struct pkt packet);    // From network layer UP to A transport
void B_timerinterrupt();            // B-side code that is called on interrupt
void B_init();                      // Simulator calls this at initialization
int genchecksum(struct pkt packet);  // Generates a checksum based on Fletchers algorithm
void pktqueuetest();
void pktqueuetest2();


/*
 * PROTOTYPES - These are in Project3.c
 *
 * These are routines that must be visible from student.c so that the code
 * written by students can get to these routines.
 * */

/*
 * format:  startTimer(calling_entity, increment), 
 * 1st argument: calling_entity is either AENTITY or BENTITY
 * 2nd argument: increment is a double value indicating the amount of time 
 * that will pass before the timer interrupts. 
 * A's timer should only be started (or stopped) by A-side routines, 
 * and similarly for the B-side timer. 
 */
void startTimer(int AorB, double increment);

/*
 * stopTimer(calling_entity), 
 * 1st argument: calling_entity is either AENTITY or BENTITY
 */
void stopTimer( int AorB );

/*
 * getClockTime()
 * Get the current simulation time - note that the call returns a double
 *   representing that time - it's not the time that's on your real clock.
 */
double  getClockTime( );

/*
 * tolayer3( calling_entity, packet). 
 * 1st argument: calling_entity is either AENTITY or BENTITY
 * 2nd argument: packet is a structure of type pkt. 
 * Calling this routine will cause the packet to be sent into the network, 
 * destined for the other entity.
 */
void tolayer3( int  AorB, struct pkt packet );

/*
 * Send message from your transport later to the application layer.
 * tolayer5( calling_entity, message). 
 * 1st argument: calling_entity is either AENTITY or BENTITY
 * 2nd argument: message is a structure of type msg. 
 *
 */
void tolayer5( int AorB, struct msg datasent);

/*
 * getTimerStatus( calling_entity), 
 * 1st argument: calling_entity is either AENTITY or BENTITY
 *
 * Returns the current state of the timer - TRUE if running, FALSE if not
 */
int getTimerStatus( int AorB );

/********************************************************************
 * ******************************************************************/

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

void queuepkt(struct pkt packet, Queue *new_queue);