#ifndef ANALOG_IN_H
#define ANALOG_IN_H

class AnalogIn {
	private:
		int channel;
	
	public:
		AnalogIn(int ch);
		static void initialize();
		unsigned short read();
};

#endif
