
#define F_CPU 16000000L


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

void init(void) {
	cli();
	DDRB |= (1 << DDB7); //enable onboard LED
	
	DDRB |= (1 << DDB5); // enable clock pin D11
	DDRB |= (1 << DDB4); // enable latch pin D10
	DDRH |= (1 << DDH6); // enable OE pin D9
	
	DDRA |= (1 << PA2); // Enable pin 23 OUT
	DDRA |= (1 << PA3); // Enable pin 24 OUT
	DDRA |= (1 << PA4); // Enable pin 25 OUT
	DDRA |= (1 << PA5); // Enable pin 26 OUT
	DDRA |= (1 << PA6); // Enable pin 27 OUT
	DDRA |= (1 << PA7); // Enable pin 28 OUT
	
	DDRF |= (1 << PF0); // enable A0 out (A)
	DDRF |= (1 << PF1); // enable A1 out (B)
	DDRF |= (1 << PF2); // enable A2 out (C)
	DDRF |= (1 << PF3); // enable A3 out (D)
	
	sei();
	
}

int main(void)
{
	init();
	int row = 0;
	while (1)
	{
		// let's start by lighting up just one row
		// Switch latches off
		PORTB &= ~(1 << PB4); // disable latch
		PORTH &= ~(1 << PH6); //enable output enable
		
		// 1. Clock in data
		
		
		
		for (int i = 0; i < 32; i++) { //32b shift register, so 64 switches
			
			//if (i == 0) {
			PORTA |= (1 << PA2); // enable R1
			PORTA |= (1 << PA3); // enable G1
			PORTA |= (1 << PA4); // enable B1
			PORTA |= (1 << PA5); // enable R2
			PORTA |= (1 << PA6); // enable G2
			PORTA |= (1 << PA7); // enable B2
			//} else {
			//	PORTA &= ~(1 << PA2); // enable R1
			//	PORTA &= ~(1 << PA3); // enable G1
			//	PORTA &= ~(1 << PA4); // enable B1
			//	PORTA &= ~(1 << PA5); // enable R2
			//	PORTA &= ~(1 << PA6); // enable G2
			//	PORTA &= ~(1 << PA7); // enable B2
			// }
			
			_delay_us(5);
			PORTB |= (1 << PB5); // switch clock bit
			_delay_us(5);
			PORTB &= ~(1 << PB5); // switch clock bit
		}
		
		// 2. Switch latches on
		PORTB |= (1 << PB4); // enable latch
		PORTH |= (1 << PH6); //enable output enable
		
		// 3. Set appropriate row
		//PORTF &= ~(1 << PF0); // set A to low
		//PORTF &= ~(1 << PF1); // set B to low
		//PORTF &= ~(1 << PF2); // set C to low
		//PORTF &= ~(1 << PF3); // set D to low
		
		
		if (((unsigned int) row >> 0) & 1) {
			PORTF |= (1 << PF3);
			} else {
			PORTF &= ~(1 << PF3);
		}
		if (((unsigned int) row >> 1) & 1) {
			PORTF |= (1 << PF2);
			} else {
			PORTF &= ~(1 << PF2);
		}
		if (((unsigned int) row >> 2) & 1) {
			PORTF |= (1 << PF1);
			} else {
			PORTF &= ~(1 << PF1);
		}
		if (((unsigned int) row >> 3) & 1) {
			PORTF |= (1 << PF0);
			} else {
			PORTF &= ~(1 << PF0);
		}
		
		_delay_us(10);
		// 4. Switch latches off
		PORTB &= ~(1 << PB4); // disable latch
		PORTH &= ~(1 << PH6); //disable output enable
		
		
		PORTB ^= (1 << PB7); // switch onboard LED

		row++;
		
		if (row >= 16) {
			row = 0;
		}
	}
	
	return 0;
}

