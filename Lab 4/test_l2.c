#include "3140_concur.h"
#include "utils.h"
#include "lock.h"


lock_t a;
void p1(void){
	int i= 0;
	while(i < 1){
		/*NCS*/
		delay();
		i++;
		/*CS*/
		l_lock(&a);
		LEDRed_Toggle();
		delay();
		LEDRed_Toggle();
		delay();
		l_unlock(&a);
	}
}

void p2(void){
	int i= 0;
	/*CS*/
	l_lock(&a);
	while(i < 6){
		delay();
		i++;
		LEDBlue_Toggle();
		delay();
		LEDBlue_Toggle();
		delay();
	}
	l_unlock(&a);
}

void p3(void){
	int i= 0;
	l_lock(&a);
	while(i < 3){
		/*NCS*/
		delay();
		i++;
		LEDGreen_Toggle();
		delay();
		LEDGreen_Toggle();
		delay();
	}
	l_unlock(&a);
}

//the blue one first toggle 6 times, and then the green one toggle 3 times, and then the red one toggle 3 times. 
int main(void){
	LED_Initialize();           /* Initialize the LEDs           */	

	l_init (&a);

	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p3,20) < 0) {
	 	return -1;
	}

	
	process_start();
	LEDGreen_On();

	while(1);
	return 0;	
}
