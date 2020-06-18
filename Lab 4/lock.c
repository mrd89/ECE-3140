#include "shared_structs.h"
#include "3140_concur.h"
#include "process.h"
#include <fsl_device_registers.h>

/**
 * Initialises the lock structure
 *
 * @param l pointer to lock to be initialised
 */
void l_init(lock_t* l){
	l -> state = 0; // initialize the lock is not in use
	l -> blocked_processqueue = NULL;
}

//enqueue the lock's blocked queue, add a process to the end of blocked queue
static void enqueue(process_t* proc, process_t** blockedqueue) {
	if (*blockedqueue == NULL) { //proc is the first process on the quenue now
		*blockedqueue = proc;
	} 
	else{
		process_t* temp = *blockedqueue; 
		while(temp ->next!= NULL){ //there are still have processes in the queue
			temp = temp -> next;
		}
		temp -> next = proc;
	}
	proc -> next = NULL;
}

//dequeue the lock's blocked queue. remove the first process from the queue
static process_t* dequeue(process_t** blockedqueue){
	if (*blockedqueue == NULL){ //nothing in the queue
		return NULL;
	}
	else{ //move to the next process on the queue
		process_t * proc = *blockedqueue;
	  *blockedqueue = proc->next;
		//proc -> next = null;
	  return proc;
	}
}

void l_lock (lock_t* l){
	PIT->CHANNEL[0].TCTRL = 1; //disable the interrupts
	// if the lock is already used 
	if (l -> state == 1){
		enqueue(current_process, &(l -> blocked_processqueue)); //add the process to l's blocked queue
		current_process -> blocked = 1; //set bloack field
		process_blocked(); //block the process
		}
	
	//give the lock to the process
	l -> state = 1; 
	PIT->CHANNEL[0].TCTRL = 3; //enable the interrupts
}

void l_unlock (lock_t *l){
	PIT->CHANNEL[0].TCTRL = 1;    //disable the interrupts
	if (l -> blocked_processqueue == NULL ){
		//no other process want to take the lock
		l -> state = 0;
	}
	else {
		//there are other process want to take the lock
		l -> state = 0;
		process_t* temp = dequeue(&(l -> blocked_processqueue)); // remove the process from the blocked queue
		temp -> blocked = 0;  // set blocked field to 0
		push_tail_process(temp); //add the process to the ready process_queue
	}
	PIT->CHANNEL[0].TCTRL = 3;    //enable interrupts
} 