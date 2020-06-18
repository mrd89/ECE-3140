#ifndef PLAYER_H
#define PLAYER_H

#include "AnalogIn.h"
#include "BusOut.h"
#include "RGBmatrixPanel.h"

class Player {
	int x, y;
	bool atEdge;
	int getCornerX(int s);
	int getCornerY(int y);
	int getXFromCorner(int cx);
	int getYFromCorner(int cy);
	
	public:
		Player();
		void update(AnalogIn* lr, AnalogIn* vd);
		void draw(RGBmatrixPanel* matrix, BusOut* ledBus);
};

#endif
