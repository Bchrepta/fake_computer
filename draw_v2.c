/************************************************************************/
// RGB MATRIX DRAW LIBRARY, V2
// WRITTEN BY MAXWELL ROLING
// 4/9/2021
// NOTE: THIS LIBRARY EXPANDS ON V1 TO OFFER ADDITIONAL ABSTRACTIONS AND FEATURES.
                                                                  
/************************************************************************/


#include "draw_v2.h"
#include "pzim_font.h"

bool R[HEIGHT][WIDTH];
bool G[HEIGHT][WIDTH];
bool B[HEIGHT][WIDTH];

int currX = 0;
int currY = 0;
int currXDir = 1;
int currYDir = 3;
int pongCounter = 0;

void initBoard(void) {
	cli();
	DDRB |= (1 << DDB7); //enable onboard LED
	
	DDRB |= (1 << DDB5); // enable clock pin D11
	DDRB |= (1 << DDB4); // enable latch pin D10
	DDRH |= (1 << DDH6); // enable OE pin D9
	
	DDRA |= (1 << PA2); // Enable pin 23 OUT R1
	DDRA |= (1 << PA3); // Enable pin 24 OUT B1
	DDRA |= (1 << PA4); // Enable pin 25 OUT G1
	DDRA |= (1 << PA5); // Enable pin 26 OUT R2
	DDRA |= (1 << PA6); // Enable pin 27 OUT B2
	DDRA |= (1 << PA7); // Enable pin 28 OUT G2
	
	DDRF |= (1 << PF0); // enable A0 out (A)
	DDRF |= (1 << PF1); // enable A1 out (B)
	DDRF |= (1 << PF2); // enable A2 out (C)
	DDRF |= (1 << PF3); // enable A3 out (D)
	
	sei();
	
	makeAllBlack();
}

// Draw works by scanning down the panel, 2 rows at a time, each row spaced 16 pixels apart.
// Once each of these 2 rows has scanned the entire panel, the display has updated.
void drawBoard(void) { 
	for (int row = 0; row < HALF_SCAN_WIDTH + 1; row++) {
		// Switch latches off
		PORTB &= ~(1 << PB4); // disable latch
		PORTH &= ~(1 << PH6); //disable output enable
		
		// 1. Clock in data: 
		for (int i = 0; i < WIDTH; i++) { // each row has 64b?? of information for each color:
			
			if (row < 16) {
				PORTA = ((R[row][i] << PA2) | (G[row][i] << PA3) | (B[row][i] << PA4) |
				 (R[row+16][i] << PA5) | (G[row+16][i] << PA6) | (B[row+16][i] << PA7));	
			} else {
				PORTA = ((0 << PA2) | (0 << PA3) | (0 << PA4) |
				 (0 << PA5) | (0 << PA6) | (0 << PA7));
			}
		
			PORTB |= (1 << PB5); // switch clock ON
			PORTB &= ~(1 << PB5); // switch clock OFF
			
		}
		
		// 2. Switch latches on
		PORTB |= (1 << PB4); // enable latch
		PORTH |= (1 << PH6); //enable output enable
		
		// 3. Set appropriate row
		if (((unsigned int) row >> 0) & 1) { PORTF |= (1 << PF0); } else { PORTF &= ~(1 << PF0); }
		if (((unsigned int) row >> 1) & 1) { PORTF |= (1 << PF1); } else { PORTF &= ~(1 << PF1); }
		if (((unsigned int) row >> 2) & 1) { PORTF |= (1 << PF2); } else { PORTF &= ~(1 << PF2); }
		if (((unsigned int) row >> 3) & 1) { PORTF |= (1 << PF3); } else { PORTF &= ~(1 << PF3); }
		
		// 4. Switch latches off
		PORTB &= ~(1 << PB4); // disable latch
		PORTH &= ~(1 << PH6); //disable output enable	

	}
}
void updateBoardPongExample(void) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			
			if (x == currX && y == currY) {
				R[y][x] = true;
				G[y][x] = true;
				B[y][x] = true;
			} else {
				R[y][x] = false;
				G[y][x] = false;
				B[y][x] = false;
			}
				
		}
	}
	
	if (pongCounter > 5) {
		pongCounter = 0;
		if (currX > 63 || currX < 0) {
			currXDir = currXDir * -1;
		}
		if (currY > 31 || currY < 0) {
			currYDir = currYDir * -1;
		}
		currX += currXDir;
		currY += currYDir;
	}
	pongCounter++;
}

void makeAllWhite(void) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			R[y][x] = true;
			G[y][x] = true;
			B[y][x] = true;
		}
	}
}

void makeAllBlack(void) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			R[y][x] = false;
			G[y][x] = false;
			B[y][x] = false;
		}
	}
}

void makeAllColor(bool Rin, bool Gin, bool Bin) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (Rin) { R[y][x] = true; } else { R[y][x] = false; }
			if (Gin) { G[y][x] = true; } else { G[y][x] = false; }
			if (Bin) { B[y][x] = true; } else { B[y][x] = false; }
		}
	}
}

void setBoard(int ** Rin, int ** Gin, int ** Bin) {
	memcpy(&R, &Rin, WIDTH * HEIGHT * sizeof(bool));
	memcpy(&G, &Gin, WIDTH * HEIGHT * sizeof(bool));
	memcpy(&B, &Bin, WIDTH * HEIGHT * sizeof(bool));
}

void fillRect(int x, int y, int height, int width, bool Rin, bool Gin, bool Bin) {
	if (x < 0 || y < 0 || height < 0 || width < 0 || x + width > WIDTH || y + height > HEIGHT) {
		return;
	} 
	for (int yIdx = y; yIdx < y + height; yIdx++) {
		for (int xIdx = x; xIdx < x + width; xIdx++) {
			if (Rin) { R[yIdx][xIdx] = true; } else { R[y][x] = false; }
			if (Gin) { G[yIdx][xIdx] = true; } else { G[y][x] = false; }
			if (Bin) { B[yIdx][xIdx] = true; } else { B[y][x] = false; }
		}
	}
}

void drawPixel(int x, int y, bool Rin, bool Gin, bool Bin) {
	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
		return;
	}
	
	if (Rin) { R[y][x] = true; } else { R[y][x] = false; }
	if (Gin) { G[y][x] = true; } else { G[y][x] = false; }
	if (Bin) { B[y][x] = true; } else { B[y][x] = false; }
	
}

// Font downloaded from https://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
// Code adapted from same source, although it was significantly modified to get it to actually work on this board
// This is a fairly time consuming and challenging thing to build from scratch, so I thought I'd borrow from 
// Someone willing to share their work, and solve more interesting problems with my limited time.
void drawChar(char c, uint8_t x, uint8_t y, bool Rin, bool Gin, bool Bin) {
	// Convert the character to an index
	c = c & 0x7F;
	if (c < ' ') {
		c = 0;
	} else {
		c -= ' ';
	}
	
	 // Draw pixels
	for (uint8_t w = 0; w < 3; w++) {
		for (uint8_t h = 0; h < 6; h++) {
			if (font[(int)c][w] & (1<<h)) {
				drawPixel(x+w, y+h, Rin, Gin, Bin);
			}
		}
	}
}

// also adapted from above source
void drawString(const char* str, uint8_t x, uint8_t y, bool Rin, bool Gin, bool Bin) {
	while (*str) {
		drawChar(*str++, x, y, Rin, Gin, Bin);
		x += CHAR_WIDTH + 1;
	}
}

void drawVerticalLine(int x, int y, int height, bool Rin, bool Gin, bool Bin) {
	// no need for conditional checks, because that is handled in drawPixel
	for (int yIdx = y; yIdx < y + height; yIdx++) {
		if (Rin) { R[yIdx][x] = true; } else { R[yIdx][x] = false; }
		if (Gin) { G[yIdx][x] = true; } else { G[yIdx][x] = false; }
		if (Bin) { B[yIdx][x] = true; } else { B[yIdx][x] = false; }
	}
}

void drawHorizontalLine(int x, int y, int width, bool Rin, bool Gin, bool Bin) {
	// no need for conditional checks, because that is handled in drawPixel
	for (int xIdx = x; xIdx < x + width; xIdx++) {
		if (Rin) { R[y][xIdx] = true; } else { R[y][xIdx] = false; }
		if (Gin) { G[y][xIdx] = true; } else { G[y][xIdx] = false; }
		if (Bin) { B[y][xIdx] = true; } else { B[y][xIdx] = false; }

	}
}

// for diagonal lines only!!
// This one is pretty cool: it uses Bresenham's line algorithm to efficiently compute the path of 
// a line to draw.
// (x1, y1) is the leftmost point and (x2, y2) is the rightmost point.
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLine(int x1, int y1, int x2, int y2, bool Rin, bool Gin, bool Bin) {
	if (x1 < 0 || y1 < 0 || x1 >= WIDTH || y1 >= HEIGHT || x2 < 0 || y2 < 0 || x2 >= WIDTH || y2 >= HEIGHT) {
		return;
	}
	int yDiff = y2 - y1;
	int xDiff = x2 - x1;
	int m = 2 * yDiff;
	int serr;
	if (yDiff < 0) {
		serr = xDiff - m;
	} else {
		serr = m - xDiff;
	}
	int currY = y1;
	if (y1 <= y2) {
		for (int x = x1; x <= x2; x++) {
			if (Rin) { R[currY][x] = true; } else { R[currY][x] = false; }
			if (Gin) { G[currY][x] = true; } else { G[currY][x] = false; }
			if (Bin) { B[currY][x] = true; } else { B[currY][x] = false; }

			// the magic happens here
			serr += m;
			while (serr >= 0) { currY++; serr -= 2*(xDiff); } // Use a while loop to allow lines with steeper slope to be drawn!
		}
	} else { // deal with case where slope is actually positive (y1 > y2 means that line slopes downwards)
		for (int x = x1; x <= x2; x++) {
			if (Rin) { R[currY][x] = true; } else { R[currY][x] = false; }
			if (Gin) { G[currY][x] = true; } else { G[currY][x] = false; }
			if (Bin) { B[currY][x] = true; } else { B[currY][x] = false; }

			serr += m;
			while (serr <= 0) { currY--; serr += 2*(xDiff); } 
		}

	}
	
}