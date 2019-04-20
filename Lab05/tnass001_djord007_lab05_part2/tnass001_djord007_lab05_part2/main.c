/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 5 Exercise 2
*	Exercise Description:
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>

enum States {init, wait, inc, dec, reset, incWait, decWait, resWait} state;

void tick() {
	unsigned char tmpA = ~PINA;
	switch(state) {
		case init:
			state = wait;
			break;
			
		case wait:
			if ((tmpA & 0x03) == 0x01) {
				state = inc;
			}
			else if ((tmpA & 0x03) == 0x02) {
				state = dec;
			}
			else if ((tmpA & 0x03) == 0x03) {
				state = reset;
			}
			else if ((tmpA & 0x03) == 0x00) {
				state = wait;
			}
			break;
			
		case inc:
			state = incWait;
			break;
			
		case dec:
			state = decWait;
			break;
			
		case reset:
			state = resWait;
			break;
			
		case incWait:
			if ((tmpA & 0x03) == 0x03) {
				state = reset;
			}
			else if ((tmpA & 0x03) == 0x00) {
				state = wait;
			}
			else if ((tmpA & 0x03) == 0x01) {
				state = incWait;
			}
			break;
			
		case decWait:
			if ((tmpA & 0x03) == 0x03) {
				state = reset;
			}
			else if ((tmpA & 0x03) == 0x00) {
				state = wait;
			}
			else if ((tmpA & 0x03) == 0x02) {
				state = decWait;
			}
			break;
			
		case resWait:
			if ((tmpA & 0x03) == 0x03) {
				state = reset;
			}
			else if ((tmpA & 0x03) == 0x00) {
				state = wait;
			}
			break;
			
			default:
				break;
	}
	
	switch(state) {
		case init:
			break;
			
		case wait:
			break;
			
		case inc:
			if (PORTC < 9) { PORTC += 1; }
			break;
			
		case dec:
			if (PORTC > 0) {PORTC -= 1; }
			break;
			
		case reset:
			PORTC = 0x00;
			break;
			
		case incWait:
			break;
			
		case decWait:
			break;
			
		case resWait:
			break;
			
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	state = init;

	while(1) {
		tick();
	}
}

