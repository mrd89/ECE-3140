#include "BusOut.h"
#define NULL 0

// Initializes the pins appropriately or NULL otherwise
BusOut::BusOut(DigitalOut* x1, DigitalOut* x2, DigitalOut* x3) : 
	d1(x1), d2(x2), d3(x3), d4(NULL), d5(NULL), d6(NULL)  {}

BusOut::BusOut(DigitalOut* x1, DigitalOut* x2, DigitalOut* x3, DigitalOut* x4) : 
	d1(x1), d2(x2), d3(x3), d4(x4), d5(NULL), d6(NULL)  {}
		
BusOut::BusOut(DigitalOut* x1, DigitalOut* x2, DigitalOut* x3, 
							 DigitalOut* x4, DigitalOut* x5, DigitalOut* x6) : 
	d1(x1), d2(x2), d3(x3), d4(x4), d5(x5), d6(x6) {}

// If the DigitalOut exists, then write the corresponding bit of t to it
void BusOut::write(uint8_t t) {
	int x = t;
	if (d1 != NULL) {
		*d1 = x & 1;
	}
	x >>= 1;
	
	if (d2 != NULL) {
		*d2 = x & 1;
	}
	x >>= 1;
	
	if (d3 != NULL) {
		*d3 = x & 1;
	}
	x >>= 1;
	
	if (d4 != NULL) {
		*d4 = x & 1;
	}
	x >>= 1;
	
	if (d5 != NULL) {
		*d5 = x & 1;
	}
	x >>= 1;
	
	if (d6 != NULL) {
		*d6 = x & 1;
	}
	x >>= 1;
}
