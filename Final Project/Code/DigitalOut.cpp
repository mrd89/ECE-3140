#include "DigitalOut.h"
#define shift(bit) (1<<(bit))

// Enables the GPIO pin to be an output pin 
DigitalOut::DigitalOut(GPIO_Type* p, PORT_Type* q, int n) : port(p), pin(n) {
	q->PCR[pin] |= PORT_PCR_MUX(001);
	port->PDDR |= shift(pin);
	port->PSOR |= shift(pin);
}

// Enables clock gates for all the ports
void DigitalOut::initialize() {
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
}

// Sets the value of the pin to be either HIGH or LOW 
// Usage as x = 1 to set DigitalOut x to high.
void DigitalOut::operator=(int value) {
	if (value == 0) {
		port->PCOR |= shift(pin);
	} else {
		port->PSOR |= shift(pin);
	}
}
