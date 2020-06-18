#ifndef TIMER_H
#define TIMER_H

typedef void (*Handler) ();

class Timer {
	Handler maj, min;
	int majCount, minCount;
	
	public:
		Timer(int major, int minor, Handler majorHandler, Handler minorHandler);
		static void initialize();
		void loop();
};

#endif
