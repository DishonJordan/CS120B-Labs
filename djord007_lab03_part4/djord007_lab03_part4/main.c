    
/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 3  Exercise 4 
 *	Exercise Description: copies bits [A7:A4] to [B3:B0] and [A3:A0] to [B7:B4]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

// x: 8-bit value.   k: bit position to set, range is 0-7.   b: set bit to this, either 1 or 0
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
	//   Set bit to 1           Set bit to 0
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char temp_b = 0x00, temp_c = 0x00;
	while(1) {
		temp_b = PINA >> 4;
		temp_c = PINA << 4;

		PORTB = temp_b;
		PORTC = temp_c;


	}
	return 0;
}