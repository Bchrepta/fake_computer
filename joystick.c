#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "inc/uart.h"

#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
void InitADC(void)
{
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //ENABLE ADC, PRESCALER 128
}
uint16_t readadc(uint8_t ch)
{
	ch&=0b00000111;         //ANDing to limit input to 7
	ADMUX = (ADMUX & 0xf8)|ch;  //Clear last 3 bits of ADMUX, OR with ch
	ADCSRA|=(1<<ADSC);        //START CONVERSION
	while((ADCSRA)&(1<<ADSC));    //WAIT UNTIL CONVERSION IS COMPLETE
	return(ADC);        //RETURN ADC VALUE
}
int main(void)
{
	char a[20], b[20], c[20];
	uint16_t x,y,z;
	InitADC();         //INITIALIZE ADC
	UART_init(BAUD_PRESCALER);
	
	while(1)
	{
		x = readadc(0);      //READ ADC VALUE FROM PA.0
		y = readadc(1);      //READ ADC VALUE FROM PA.1
		z = readadc(2);
		x = x-512;
		y = y-512;
		z = z-512;
		itoa(x, a, 10);
		itoa(y, b, 10);
		itoa(z, c, 10);
		UART_putstring("x = ");
		UART_putstring(a);     
		UART_putstring("\n y = ");
		UART_putstring(b);
		UART_putstring("\n z = ");
		UART_putstring(c);
		UART_putstring("\n");
	}
}