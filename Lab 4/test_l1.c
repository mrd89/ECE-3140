#include "3140_concur.h"
#include "utils.h"
#include "lock.h"


lock_t a;
lock_t b;

void p1(void){
	int i= 0;
	while(i < 3){
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

void p2(void) {
  /* enter */	
	l_lock(&b);
	
	/*start writing*/
	delay();

	LEDBlue_Toggle();
	
	delay();
	
	LEDBlue_Toggle();	
	/*end writing*/

  /* exit */			
	l_unlock(&b);
}


//because we use two lock for this test. So some times they flash together.
int main (void){		
	LED_Initialize();           /* Initialize the LEDs           */	
	l_init (&a);
	l_init (&b);
 
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	
	process_start();
 
	LEDGreen_On();

	while(1);
}