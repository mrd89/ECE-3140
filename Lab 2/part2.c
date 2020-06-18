#include <fsl_device_registers.h>
/*
     Main program: entry point
*/
int main (void){
	//SETUP FOR BLUE LED
  SIM->SCGC5 |= (1 << 10);
  PORTB->PCR[21] = (1<<8);
  PTB->PDDR = 1 << 21;
  PTB->PSOR = 1 << 21; // Turn off blue LED (above setup turns it on)

  // SETUP FOR GREEN LED
  SIM->SCGC5 |= (1 << 13);
  PORTE->PCR[26] = (1<<8);
  PTE->PDDR = 1 << 26;
  PTE->PSOR = 1 << 26; // Turn off green LED (above setup turns it on)

  SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
  PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK; // Set load value of zeroth PIT
  PIT-> MCR= 1;

  PIT->CHANNEL[0].TCTRL = 3; // Enable interrupts and timer

  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 2) */


  while(1){
		for (int i=0; i<2000000;i++){
        //loop for about 1 second
   	}
   	PTB->PTOR = 1 << 21; // Toggle blue LED

  }

}


void PIT0_IRQHandler(void){
	PTE->PTOR = 1 << 26; // Turn green LED on
  for (int i=0; i< 200000; i++){
   // loop for .1 second
  }
  PTE->PTOR = 1 << 26; // Turn green LED off
  PIT->CHANNEL[0].TFLG = 1; // Clear flag

}
