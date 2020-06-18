#include "3140_concur.h"
#include <stdlib.h>
#include <fsl_device_registers.h> 

// Structure that stores all the necessary data to 
//represent and later restore a process 
struct process_state{
	unsigned int *sp;  //stack pointer
	int size_stack; //size of stack
	unsigned int *orsp; //original stack pointer
	process_t* next; //next process
};

typedef struct process_state process_t; //typedef'ed as process_t 
//global variable
process_t *process_queue= NULL; //the first ready process that is not running
process_t *current_process= NULL; //the process that is currently running

//add a process_state to the queue
void enqueue(process_t *proc){
	if(process_queue == NULL){
		//proc is the first process on the quenue now
		process_queue = proc;
		process_queue -> next = NULL;
	}
	else{
		process_t *temp = process_queue;
		while(temp -> next != NULL){
			temp = temp -> next;
		}
		temp -> next = proc;
		proc -> next = NULL;
	}
}

//remove the first process from the queue
void dequeue(){
	if (process_queue != NULL){
		process_queue = process_queue -> next;
	}
	else{
		process_queue = NULL;
	}
}

// create the process_state data structure, and save it to your process queue 
int process_create(void (*f)(void),int n){
	NVIC_DisableIRQ(PIT0_IRQn); //disable interrupts during process_stack_init
	unsigned int *sp = process_stack_init(*f,n);
	NVIC_EnableIRQ(PIT0_IRQn); //enable interrupts after process_stack_init is done
	
	if(sp == NULL) return -1; //if malloc failed	
	process_t *proc = (process_t*) malloc(sizeof(process_t));
	if (proc == NULL) {
		process_stack_free(proc->orsp, proc->size_stack);
		return -1;
	}  //check if malloc was sucessful
	
	
	proc -> sp = sp;
	proc -> orsp = sp;
	proc -> size_stack = n;
	proc -> next = NULL;
	
	enqueue(proc);
	return 0;
}

//set up and enable PIT channel 0 timer
void process_start(void){  
	NVIC_EnableIRQ (PIT0_IRQn);
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; //enable clock to PIT module
	PIT->CHANNEL[0].TFLG = 1;
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK/100;
	PIT->CHANNEL[0].TCTRL |= 3;
	
	process_begin();
}

unsigned int * process_select(unsigned int * cursp){
	if ( cursp == NULL){ //if current stack pointer is null
		if(current_process == NULL && process_queue == NULL){
			//no processes ready, return NULL
			return NULL;
		}
		else{
			if (current_process != NULL){
				//the current process is done running, switch to next process
				NVIC_DisableIRQ(PIT0_IRQn); //disable interrupts during process_stack_init
				process_stack_free(current_process->orsp, current_process->size_stack); 
				NVIC_EnableIRQ(PIT0_IRQn); //enable interrupts after process_stack_init is done
				free(current_process);	
				if(process_queue==NULL){
					current_process= NULL;
					return NULL;
				}
				//elsee change the process queue
			}
			//else no process is running but there is a ready process
		}
	}
	else{
		process_t *tmp = current_process;
		tmp->sp= cursp; //update stack pointer of process that isn't done running
		enqueue(tmp); //put current process at the back of the queue
	}
	current_process = process_queue;
	dequeue();
	if (current_process == NULL) return NULL;
	return current_process->sp;
}



