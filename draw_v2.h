#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define HEIGHT 32
#define WIDTH 64
#define SCAN_WIDTH 32
#define HALF_SCAN_WIDTH 16
#define CHAR_WIDTH 3
#define CHAR_HEIGHT 5

void initBoard(void);
void drawBoard(void);
void updateBoardPongExample(void);
void makeAllWhite(void);
void makeAllBlack(void);
void makeAllColor(bool Rin, bool Gin, bool Bin);
void setBoard(int ** Rin, int ** Gin, int ** Bin);
void fillRect(int x, int y, int height, int width, bool Rin, bool Gin, bool Bin);
void drawPixel(int x, int y, bool Rin, bool Gin, bool Bin);
void drawChar(char c, uint8_t x, uint8_t y, bool Rin, bool Gin, bool Bin);
void drawString(const char* str, uint8_t x, uint8_t y, bool Rin, bool Gin, bool Bin);
void drawVerticalLine(int x, int y, int height, bool Rin, bool Gin, bool Bin);
void drawHorizontalLine(int x, int y, int width, bool Rin, bool Gin, bool Bin);
void drawLine(int x1, int y1, int x2, int y2, bool Rin, bool Gin, bool Bin);