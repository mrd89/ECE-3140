#include "3140_concur.h"
#include "utils.h"

//Red LED blinks 2 times
void p1 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDRed_Toggle();
	}
}

//Blue LED blinks 8 times
void p2 (void)
{
	int i;
	for (i=0; i < 16; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}


//Green LED blinks 4 times
void p3 (void)
{
	int i;
	for (i=0; i < 8; i++) {
		delay ();
    LEDGreen_Toggle();
	}
}


// only one process
int main (void)
{
 LED_Initialize();
	
 if (process_create (p1,20) < 0) {
 	return -1;
 }
 process_start ();
 
 LEDGreen_On();
 
 while (1) ;

 return 0;
}
