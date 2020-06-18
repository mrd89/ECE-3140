#include <fsl_device_registers.h>


int main (void) {
  SIM->SCGC5 |= (1<<10); // Enable the clock to port B
  PORTB->PCR[22] = (1<<8); // Set up PTB22 as GPIO
  PTB->PDDR |= (1 <<22); // Enable PTB22 as output

  SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
  PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK; // Set load value of zeroth PIT
  PIT->CHANNEL[0].TCTRL = 1; //Enable the timer
  PIT->MCR = 1;

  while(1) {
    if (PIT->CHANNEL[0].TFLG == 1){
      PTB->PTOR = (1 <<22); // Toggle red LED
      PIT->CHANNEL[0].TFLG = 1;
    }
  }
}
