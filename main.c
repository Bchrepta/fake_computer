#include "draw_v2.h"

int main(void) {
	initBoard();
	// demo RGB display
	/*
	// demonstrate text
	makeAllColor(false, false, true);
	drawString("Hello, World!", 5, 5, true, true, true);
	drawString("Max was here!", 5, 15, false, false, false);
	*/
	/* 
	// draw lines
	makeAllColor(false, false, true);
	drawLine(0, 0, 63, 31, true, true, true);
	drawLine(0, 31, 63, 0, true, true, true);
	drawHorizontalLine(0, 16, 64, true, true, true);
	drawVerticalLine(32, 0, 32, true, true, true);
	*/

	while(1) {
		drawBoard();
		/*
		// pong pixel!
		updateBoardPongExample();
		*/
	}
	
	return 0;
}