
#include "utils.h"
#include "3140_concur.h"
#include "realtime.h"

 
/* Stack space for processes */
#define NRT_STACK 80
#define RT_STACK  80

/* Constants used for 'work' and 'deadline's */
realtime_t t_1msec = {0, 1};
realtime_t t_10sec = {10, 0};
realtime_t t_1sec={1,0};

/* Process start time */
realtime_t t_pRT1 = {2, 0};

 
/*------------------*/
/* Helper functions */
/*------------------*/
void shortDelay(){delay();}
void mediumDelay() {delay(); delay();}



//n realtime process that tests if busy weight works\\

void pRT1(void) {
	int i;
	for (i=0; i<5;i++){
		
		LEDBlue_On();
		mediumDelay();

		LEDBlue_Toggle();
		mediumDelay();
	}
}




//test code calling functions
int main(void) {	
	 
	LED_Initialize();

   //creates the single real time process

    if (process_rt_create(pRT1, RT_STACK, &t_pRT1, &t_10sec) < 0) { return -1; } 
   
 // start processes
	process_start();

  LED_Off();
		
  while(process_deadline_met>0) {
		LEDGreen_On();
		shortDelay();
		LED_Off();
		shortDelay();
		process_deadline_met--;
	}
	
	while (1);
	return 0;
}
