/*
 * GccApplication3.c
 *
 * Created: 4/1/2021 10:59:34 PM
 * Author : Maxwell Roling
 */ 


/************************************************************************/
// RGB MATRIX DRAW LIBRARY, V1
// WRITTEN BY MAXWELL ROLING
// 4/3/2021
// NOTE: THIS IS THE FIRST, AND MOST BASIC, VERSION OF A LIBRARY CAPABLE
// OF DRAWING TO AN RGB MATRIX. MORE IMPROVEMENTS TO COME.
                                                                  
/************************************************************************/


#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define HEIGHT 32
#define WIDTH 64
#define SCAN_WIDTH 32
#define HALF_SCAN_WIDTH 16


//long long unsigned int R[HEIGHT];
//long long unsigned int G[HEIGHT];
//long long unsigned int B[HEIGHT];
//long long unsigned int Buffer[WIDTH];
bool R[HEIGHT][WIDTH];
bool G[HEIGHT][WIDTH];
bool B[HEIGHT][WIDTH];

int currX = 0;
int currY = 0;
int currXDir = 1;
int currYDir = 3;
int counter = 0;

void init(void) {
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
}

// Draw works by scanning down the panel, 2 rows at a time, each row spaced 16 pixels apart.
// Once each of these 2 rows has scanned the entire panel, the display has updated.
void drawBoard() { //long long unsigned int R[HEIGHT], uint64_t G[HEIGHT], uint64_t B[HEIGHT], uint64_t Buffer[WIDTH]) {//int R[HEIGHT][WIDTH], int G[HEIGHT][WIDTH], int B[HEIGHT][WIDTH]) {
	for (int row = 0; row < 17; row++) { // 1/16 scan rate, this needs to be 17 though??
		// Switch latches off
		PORTB &= ~(1 << PB4); // disable latch
		PORTH &= ~(1 << PH6); //disable output enable
		
		// 1. Clock in data: 
		for (int i = 0; i < 64; i++) { // each row has 64b?? of information for each color:
			
			if (row < 16) {
				if (R[row][i] == true) {PORTA |= (1 << PA2); } else {PORTA &= ~(1 << PA2);}// enable R1
				if (G[row][i] == true) {PORTA |= (1 << PA3); } else {PORTA &= ~(1 << PA3);}// enable G1
				if (B[row][i] == true) {PORTA |= (1 << PA4); } else {PORTA &= ~(1 << PA4);}// enable B1
				
				if (R[row+16][i] == true) {PORTA |= (1 << PA5); } else {PORTA &= ~(1 << PA5);}// enable R2
				if (G[row+16][i] == true) {PORTA |= (1 << PA6); } else {PORTA &= ~(1 << PA6);}// enable G2
				if (B[row+16][i] == true) {PORTA |= (1 << PA7); } else {PORTA &= ~(1 << PA7);}// enable B2
				
			} else {
				PORTA &= ~(1 << PA2);
				PORTA &= ~(1 << PA3);
				PORTA &= ~(1 << PA4);
				PORTA &= ~(1 << PA5);
				PORTA &= ~(1 << PA6);
				PORTA &= ~(1 << PA7);
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
void updateBoard(void) {
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
	
	
	if (counter > 10) {
		counter = 0;
		if (currX > 63 || currX < 0) {
			currXDir = currXDir * -1;
		}
		if (currY > 31 || currY < 0) {
			currYDir = currYDir * -1;
		}
		currX += currXDir;
		currY += currYDir;
	}
	counter++;
}

int main(void) {
	init();
	srand(time(NULL));
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			R[y][x] = true;
			G[y][x] = true;
			B[y][x] = true;
			/*
			if (x % 2 == 0 && y % 2 == 0) {
				R[y][x] = true;
				G[y][x] = true;
				B[y][x] = true;
			} else if (x % 2 == 0) {
				R[y][x] = true;
				G[y][x] = false;
				B[y][x] = false;
			} else if (y % 2 == 0) {
				R[y][x] = false;
				G[y][x] = false;
				B[y][x] = true;
			} else {
				R[y][x] = false;
				G[y][x] = true;
				B[y][x] = false;
			} */
			
			
			
		}
	}
	
	
	while(1) {
		PORTB ^= (1 << PB7); // switch onboard LED
		drawBoard();
		//_delay_ms(20);
		updateBoard();
	}
	return 0;
}