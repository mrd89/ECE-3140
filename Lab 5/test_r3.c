
#include "utils.h"
#include "3140_concur.h"
#include "realtime.h"

/* Stack space for processes */
#define NRT_STACK 20
#define RT_STACK  20
 

//defines the realtime time for calling processes
realtime_t start0 = {2,0};
realtime_t start1 = {0,1};
realtime_t start2 = {1,0};

// real time variables
realtime_t t_deadline = {10,0};


//simple functions
void shortDelay(){delay();}
void mediumDelay() {delay(); delay();}


void pRT0 (void){
	int i;
	for (i=0;i<3;i++){
		LEDBlue_On();
		mediumDelay();
		LEDBlue_Toggle();
		shortDelay();
		
	}
}
void pRT1 (void){
	int i;
	for (i=0;i<3;i++){
		LEDRed_On();
		mediumDelay();
		LEDRed_Toggle();
		shortDelay();
		
	}
}
void pRT2 (void){
	int i;
	for (i=0;i<3;i++){
		LEDGreen_On();
		mediumDelay();
		LEDGreen_Toggle ();
		shortDelay();
	}
}

void pNRT(void) {
	int i;
	for (i=0;i<2;i++){
		LEDBlue_On();
		shortDelay();
		LEDBlue_Toggle();
		LEDRed_On();
		shortDelay();
		LEDRed_Toggle();
		shortDelay();
	}
}
	
// main test code
/*

*/

int main(void) {	
	 
	LED_Initialize();
	
	// Adding Test Processes to queue
		if(process_rt_create(pRT0,NRT_STACK, &start0,&t_deadline) <0) { return -1;}
		if(process_rt_create(pRT1,NRT_STACK, &start1,&t_deadline) <0) { return -1;}
		if(process_rt_create(pRT2,NRT_STACK, &start2,&t_deadline) <0) { return -1;}
		if (process_create(pNRT,NRT_STACK) < 0) {return -1;}
  //Starts Processes
		
		
	process_start();

  LED_Off();
		
	//code for clearly outputting delays
	mediumDelay();
	mediumDelay();		
		
  while(process_deadline_miss>0) {
		LEDGreen_On();
		shortDelay();
		LED_Off();
		shortDelay();
		process_deadline_miss--;
	}
	
	mediumDelay();
	mediumDelay();
	
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
