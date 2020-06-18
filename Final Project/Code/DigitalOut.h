#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#include <MK64F12.h>

class DigitalOut {
	GPIO_Type* port;
	int pin;
	
	public:
		DigitalOut(GPIO_Type* p, PORT_Type* q, int n);
		static void initialize();
		void operator=(int value);
};

#endif
