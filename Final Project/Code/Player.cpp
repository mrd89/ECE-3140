#include "Player.h"

Player::Player() : x(0), y(0), atEdge(true) {}

// Converts a top x coordinate to the bottom x coordinate of the opposite corner
int Player::getCornerX(int s) {
	return s + 11;
}

// Converts a top y coordinate to the bottom y coordinate of the opposite corner
int Player::getCornerY(int s) {
	return y + 4;
}

// Converts a bottom x coordinate to the top x coordinate of the opposite corner
int Player::getXFromCorner(int cx) {
	return cx - 11;
}

// Converts a bottom y coordinate to the top y coordinate of the opposite corner
int Player::getYFromCorner(int cy) {
	return cy - 4;
}

// Draws the player to the screen, lighting up the onboard LED if at the edge
void Player::draw(RGBmatrixPanel* matrix, BusOut* ledBus) {
	if (atEdge) {
		ledBus->write(0);
	} else {
		ledBus->write(15);
	}
	
	int cx = x + 2, cy = y + 2;
	int bodyY = cy; int bodyXTop = x + 5; int bodyXBottom = bodyXTop + 3;
	int armStartX = bodyXTop + 2; int armEndX = armStartX - 2; 
	int armStartY = cy; int leftArmEndY = armStartY + 2;  int rightArmEndY = armStartY - 2;
	int legStartX = bodyXBottom + 1; int legEndX = legStartX + 2;
	int legStartY = cy; int leftLegEndY = legStartY + 2; int rightLegEndY = legStartY -2;
	
	matrix->fillScreen(matrix->Color333(0,0,0));
	matrix->fillCircle(cx, cy, 2, matrix->Color888(0, 76, 153));
	matrix->drawLine(bodyXTop, bodyY, bodyXBottom, bodyY, matrix->Color888(100, 76, 153));
	matrix->drawLine(armStartX, armStartY, armEndX, leftArmEndY, matrix->Color888(255, 0, 0));
	matrix->drawLine(armStartX, armStartY, armEndX, rightArmEndY, matrix->Color888(255, 0, 0));
	matrix->drawLine(legStartX, legStartY, legEndX, leftLegEndY, matrix->Color888(180, 0, 180));
	matrix->drawLine(legStartX, legStartY, legEndX, rightLegEndY, matrix->Color888(180, 0, 180));
}

// Updates the players position given the two analog potentiometers
void Player::update(AnalogIn* vd, AnalogIn* lr) {
	int yInput = lr->read();
	int xInput = vd->read();
	atEdge = false;
	
	if (xInput > 0xFFFF * 7 / 8) {
			x--;
	} else if (xInput < 0xFFFF / 8) {
			x++;
	}
	
	if (yInput > 0xFFFF * 7 / 8) {
			y--;
	} else if (yInput < 0xFFFF / 8) {
			y++;
	}
	
	if (x <= 0) {
		x = 0;
		atEdge = true;
	} else if (getCornerX(x) >= 31) {
		x = getXFromCorner(31);
		atEdge = true;
	}
	
	if (y <= 0) {
		y = 0;
		atEdge = true;
	} else if (getCornerY(y) >= 31) {
		y = getYFromCorner(31);
		atEdge = true;
	}
}
