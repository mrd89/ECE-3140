#include "3140_concur.h"
#include "utils.h"

//Red LED blinks 5 times
void p1 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDRed_Toggle();
	}
}

//Blue LED blinks 5 times
void p2 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}


//Green LED blinks 5 times
void p3 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDGreen_Toggle();
	}
}

//Blue LED blinks 2 times
void p4 (void)
{
	int i;
	for (i=0; i < 4; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}

// Three big process. First, RED, BLUE, Green LED's blink simultaneously 5 times. Second, RED, BLUE LED blink simultaneously 5 times
// Finally, only red led blink 2 times
int main (void)
{
 LED_Initialize();
 
 if (process_create (p1,20) < 0) {
 	return -1;
 }

 if (process_create (p2,20) < 0) {
 	return -1;
 }
 
 if (process_create (p3,20) < 0) {
 	return -1;
 }
 
 process_start ();
 
 LEDGreen_On();
 delay();
 LED_Off();
 
 if (process_create (p1,20) < 0) {
 	return -1;
 }

 if (process_create (p2,20) < 0) {
 	return -1;
 }
 process_start();
 LEDGreen_On();
 delay();
 LED_Off();
 
 if (process_create (p4,20) < 0) {
 	return -1;
 }
 process_start();
 LEDGreen_On();
 while (1) ;

 return 0;
}
