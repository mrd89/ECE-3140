#include "AnalogIn.h"
#include <fsl_device_registers.h>

AnalogIn::AnalogIn(int ch) : channel(ch) {}

// Enable the clock gates for ADC0 and ports B/C
void AnalogIn::initialize() {
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// Configure ADC0 according to the reference manual
	ADC0->CFG1 = 0;
	ADC0->CFG1 |= ADC_CFG1_MODE(3);
	ADC0->SC1[0] |= ADC_SC1_ADCH(31);
}

// Taken from the reference guide as a way of reading from the channel
unsigned short AnalogIn::read() {
	ADC0->SC1[0] = channel & ADC_SC1_ADCH_MASK;
	while(ADC0->SC2 & ADC_SC2_ADACT_MASK);
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}
