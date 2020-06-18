#include <fsl_device_registers.h>
#include "DigitalOut.h"
#include "BusOut.h"
#include "RGBmatrixPanel.h"
#include "AnalogIn.h"
#include "Timer.h"
#include "Player.h"

BusOut* ledBus;
RGBmatrixPanel* matrix;
AnalogIn *lr, *vd;
Player* player;

// The main game loop, called once every major cycle
void loop(){
	player->update(vd, lr);
	player->draw(matrix, ledBus);
}

// The update display function, called once every minor cycle
void updateDisplay() {
	matrix->updateDisplay();
}

// The main program for the game
int main(){
		// Setup LED Matrix
		DigitalOut::initialize();
		DigitalOut r1(PTE, PORTE, 24);
		DigitalOut g1(PTD, PORTD, 2);
		DigitalOut b1(PTD, PORTD, 1);
		DigitalOut r2(PTE, PORTE, 25);
		DigitalOut g2(PTD, PORTD, 0);
		DigitalOut b2(PTD, PORTD, 3);
		DigitalOut a(PTB, PORTB, 10);
		DigitalOut b(PTB, PORTB, 11);
		DigitalOut c(PTC, PORTC, 11);
		DigitalOut d(PTC, PORTC, 10);
		DigitalOut clk(PTC, PORTC, 17);
		DigitalOut lat(PTB, PORTB, 9);
		DigitalOut oe(PTC, PORTC, 16);
		matrix = new RGBmatrixPanel(r1,r2,g1,g2,b1,b2,a,b,c,d,clk,lat,oe);
		
		// Setup onboard LED Bus
		DigitalOut redLed(PTB, PORTB, 22);
		DigitalOut blueLed(PTB, PORTB, 21);
		DigitalOut greenLed(PTE, PORTE, 26);
		ledBus = new BusOut(&redLed, &blueLed, &greenLed);
		
		// Setup Analog Inputs for joystick
		AnalogIn::initialize();
		lr = new AnalogIn(12);
		vd = new AnalogIn(13);
		
		// Create player object
		player = new Player();
		
		// Setup main game loop, 
		// Call update display for every minor cycle and game loop for every major cycle
		Timer::initialize();
		Timer gameTimer(DEFAULT_SYSTEM_CLOCK / 6000, 400, &updateDisplay, &loop);
		gameTimer.loop();
}  

