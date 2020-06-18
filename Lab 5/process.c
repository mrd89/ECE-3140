#include "3140_concur.h"
#include <fsl_device_registers.h>
#include "realtime.h"

struct process_state {
	unsigned int n;
	struct process_state * next;
	unsigned int * sp;
	unsigned int * orig_sp;
	realtime_t* deadline;
	realtime_t* start;
};

realtime_t current_time;

int process_deadline_met = 0;
int process_deadline_miss = 0;

void PIT1_IRQHandler() {
	current_time.msec++;
	if (current_time.msec == 1000) {
		current_time.sec++;
		current_time.msec = 0;
	}
	PIT->CHANNEL[1].TFLG = 1;
}

/* the currently running process. current_process must be NULL if no process is running,
    otherwise it must point to the process_t of the currently running process
*/
process_t * current_process = NULL; 
process_t * process_queue = NULL;
process_t * process_tail = NULL;
		

unsigned int get_time(realtime_t* t) {
	int sec = t->sec;
	int msec = t->msec;
	return sec * 1000 + msec;
}

static process_t * pop_earliest_deadline_process() {
	// find the current process that can be started and has the earliest deadline
	if (process_queue == NULL) {
			// no processes to execute
			return NULL;
	}
	
	int curr_time = get_time(&current_time);
	int min_start = -1;
	
	process_t* curr = process_queue;
	process_t* prev = NULL;
	
	// loop through queue
	while (curr != NULL) {
		if (curr->start == NULL || curr_time >= get_time(curr->start)) {
			// remove curr and return it
			if (prev == NULL) {
				process_queue = curr->next;
			} else {
				prev->next = curr->next;
			}
			
			if (process_tail == curr) {
				process_tail = prev;
			}

			curr->next = NULL;
			return curr;
		}
		
		if (min_start < 0 || get_time(curr->start) < min_start) {
			min_start = get_time(curr->start);
		}
		prev = curr;
		curr = curr->next;
	}
	
	// no process was selected so we need to busy wait until curr_time is >= min_start
	__enable_irq();
	
	while(1) {
		__disable_irq();
		curr_time = get_time(&current_time);
		
		if (curr_time >= min_start) {
			return pop_earliest_deadline_process();
		}
		
		__enable_irq();
	}
}


// 0 if equal
// positive if p1 > p2
// negative if p2 > p1
int compare_process(process_t* p1, process_t* p2) {
	if (p1->start == NULL && p2->start == NULL) {
		return 0;
	} else if (p1->start == NULL) {
		return 1;
	} else if (p2->start == NULL) {
		return -1;
	}
	
	int p1time = get_time(p1->start) + get_time(p1->deadline);
	int p2time = get_time(p2->start) + get_time(p2->deadline);
	return p1time - p2time;
}

static void insert_sorted_process(process_t *proc) {
	process_t * curr = process_queue;
	process_t * prev = NULL;
	
	// make curr point to the first process greater than proc
	// curr->next != null && deadline of proc >= deadline of curr->next
	while (curr != NULL && (compare_process(curr, proc) < 0 || (curr->start == NULL && compare_process(curr, proc) == 0))) {
		prev = curr;
		curr = curr->next;
	}
	
	// insert proc before curr
	if (curr == NULL) {
		if (process_tail == NULL) {
			process_queue = proc;
			process_tail = proc;
			proc->next = NULL;
		} else {
			process_tail->next = proc;
			proc->next = NULL;
			process_tail = proc;
		}
	} else {
		if (prev == NULL) {
			proc->next = process_queue;
			process_queue = proc;
		} else {
			prev->next = proc;
			proc->next = curr;
		}
	}
}

static void process_free(process_t *proc) {
	process_stack_free(proc->orig_sp, proc->n);
	free(proc);
}

int process_rt_periodic(void (*f)(void), int n, realtime_t *start, realtime_t *deadline, realtime_t *period) {
	return 0;
}

int process_rt_create(void (*f)(void), int n, realtime_t* start, realtime_t* deadline) {
	unsigned int *sp = process_stack_init(f, n);
	if (!sp) return -1;
	
	process_t *proc = (process_t*) malloc(sizeof(process_t));
	if (!proc) {
		process_stack_free(sp, n);
		return -1;
	}
	
	proc->sp = proc->orig_sp = sp;
	proc->n = n;
	proc->deadline = deadline;
	proc->start = start;
	
	insert_sorted_process(proc);
	return 0;
}

/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/
unsigned int * process_select (unsigned int * cursp) {
	if (cursp) {
		// Suspending a process which has not yet finished, save state and make it the tail
		current_process->sp = cursp;
		insert_sorted_process(current_process);
	} else {
		// Check if a process was running, free its resources if one just finished
		if (current_process) {
			if (current_process->start != NULL) {
				if (get_time(current_process->deadline) + get_time(current_process->start) >= get_time(&current_time)) {
					process_deadline_met++;
				} else {
					process_deadline_miss++;
				}
			}
			
			process_free(current_process);
		}
	}
	
	// Select the new current process from the front of the queue
	current_process = pop_earliest_deadline_process();
	
	if (current_process) {
		// Launch the process which was just popped off the queue
		return current_process->sp;
	} else {
		// No process was selected, exit the scheduler
		return NULL;
	}
}

/* Starts up the concurrent execution */
void process_start (void) {
	NVIC_SetPriority(SVCall_IRQn, 2);
	NVIC_SetPriority(PIT0_IRQn, 1);
	NVIC_SetPriority(PIT1_IRQn, 0);
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK / 10;
	NVIC_EnableIRQ(PIT0_IRQn);
	
	current_time.sec = 0;
	current_time.msec = 0;
	PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK / 1000;
	PIT->CHANNEL[1].TCTRL = 0b11;
	PIT->CHANNEL[1].TFLG = 1;
	NVIC_EnableIRQ(PIT1_IRQn);
	
	// Bail out fast if no processes were ever created
	if (!process_queue) return;
	process_begin();
}

/* Create a new process */
int process_create (void (*f)(void), int n) {
	unsigned int *sp = process_stack_init(f, n);
	if (!sp) return -1;
	
	process_t *proc = (process_t*) malloc(sizeof(process_t));
	if (!proc) {
		process_stack_free(sp, n);
		return -1;
	}
	
	proc->sp = proc->orig_sp = sp;
	proc->n = n;
	proc->deadline = NULL;
	proc->start = NULL;
	
	insert_sorted_process(proc);
	return 0;
}
