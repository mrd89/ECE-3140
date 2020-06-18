#include <fsl_device_registers.h>
#include "Timer.h"

// Setup PIT to count every minor cycle
Timer::Timer(int minor, int major, Handler minorHandler, Handler majorHandler) :
	maj(majorHandler), min(minorHandler), majCount(major), minCount(minor) {
	PIT->CHANNEL[0].LDVAL = minor;
}
	
// Enable clock gates and setup the PIT module
void Timer::initialize() {
		SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
		PIT->MCR = 0;
		PIT->CHANNEL[0].TCTRL = 1;
		PIT->CHANNEL[0].TFLG = 1;
}

// Loop for ever, calling the minor handler every minor cycle and 
// the major handler every major cycle
void Timer::loop() {
	int time = 0;
	
	while (true) {
		if (PIT->CHANNEL[0].TFLG != 1) continue;
		min();
		
		if (time == majCount) {
			maj();
			time = 0;
		}
		
		time++;
		PIT->CHANNEL[0].TFLG = 1;
	}
}
