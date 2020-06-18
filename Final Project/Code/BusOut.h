#ifndef BUS_OUT_H
#define BUS_OUT_H

#include <stdint.h>
#include "DigitalOut.h"

class BusOut {
	// Pins in the bus
	DigitalOut *d1, *d2, *d3, *d4, *d5, *d6;
	
	public:
		BusOut(DigitalOut* d1, DigitalOut* d2, DigitalOut* d3);
		BusOut(DigitalOut* x1, DigitalOut* x2, DigitalOut* x3, DigitalOut* x4);
		BusOut(DigitalOut* d1, DigitalOut* d2, DigitalOut* d3, 
					 DigitalOut* d4, DigitalOut* d5, DigitalOut* d6);
	
		// Write each bit to the corresponding digital out
		void write(uint8_t t);
};

#endif
