
#include "utils.h"
#include "3140_concur.h"
#include "realtime.h"

/* Stack space for processes */
#define NRT_STACK 80
#define RT_STACK  80
 

//defines the realtime time for calling processes
realtime_t t_1msec = {0, 1};
realtime_t t_10sec = {10, 0};
realtime_t t_1sec= {1,0};


// real time variables
realtime_t t_pRT1 = {1, 0};


//simple functions
void shortDelay(){delay();}
void mediumDelay() {delay(); delay();}


void pNRTRed (void){
	int i;
	for (i=0;i<3;i++){
		LEDRed_On();
		shortDelay();
		LEDRed_Toggle();
		shortDelay();
	}

}


void pNRTBlue(void){
	int i;
	for (i=0;i<4;i++){
		LEDBlue_On();
		shortDelay();
		LEDBlue_Toggle();
		shortDelay();
	}
}




// main test code
/* this code will call the blue and red LED's 3 times at the same time followed by one final Blue LED*/

int main(void) {	
	 
	LED_Initialize();
	
	// Adding Test Processes to queue
	
    if (process_create(pNRTRed, NRT_STACK) < 0) { return -1; }
		if (process_create(pNRTBlue,NRT_STACK) < 0) {return -1;}
			
   
  //Starts Processes
		
		
	process_start();

  LED_Off();
		
		//code for clearly outputting delays
  while(process_deadline_miss>0) {
		LEDGreen_On();
		shortDelay();
		LED_Off();
		shortDelay();
		process_deadline_miss--;
	}
	
	while(process_deadline_met>0) {
		LEDGreen_On();
		shortDelay();
		LED_Off();
		shortDelay();
		process_deadline_met--;
	}
	
	/* Hang out in infinite loop (so we can inspect variables if we want) */ 
	while (1);
	return 0;
}
