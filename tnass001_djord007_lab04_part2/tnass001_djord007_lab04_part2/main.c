/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 4  Exercise 2
*	Exercise Description:
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>

enum States {start, init, inc, dec, w1, w2, w3, res_w, res_c, } state;

void tick() {
	switch(state) {	
		case start:
			state = init;
			break;
			
		case init:
			if ((PINA & 0x03) == 0x01) {
				state = inc;
			}
			else if((PINA & 0x03) == 0x02) {
				state = dec;
			}
			else if ((PINA & 0x03) == 0x03) {
				state = res_c;
			}
			break;
			
		case inc:
			state = w1;
			break;
			
		case w1:
			if((PINA & 0x03) == 0x01) {
				state = w1;
			}
			else if((PINA & 0x03) == 0x00) {
				state = res_w;
			}
			else if((PINA & 0x03) == 0x03) {
				state = res_c;
			}
			else if ((PINA & 0x03) == 0x02) {
				state = dec;
			}
			break;
			
		case dec:
			state = w2;
			break;
		
		case w2:
			if((PINA & 0x03) == 0x02) {
				state = w2;
			}
			else if((PINA & 0x03) == 0x00) {
				state = res_w;
			}
			else if((PINA & 0x03) == 0x03) {
				state = res_c;
			}
			else if ((PINA & 0x03) == 0x01) {
				state = inc;
			}
			break;
			
		case res_w:
			if ((PINA & 0x03) == 0x01) {
				state = inc;
			}
			else if ((PINA & 0x03) == 0x02) {
				state = dec;
			}
			else if ((PINA & 0x03) == 0x03) {
				state = res_c;
			}
			break;
			
		case res_c:
			state = w3;
			break;
			
		case w3:
			state = w3;
			if ((PINA & 0x03) == 0x00) {
				state = init;
			}
			break;
			
		default:
			state = init;
	}
	
	switch(state) {
		case init:
			break;
			
		case inc:
			if (PORTC < 9) { PORTC += 1; }
			break;
		
		case w1:
			break;
			
		case dec:
			if (PORTC > 0) { PORTC -= 1; }
			break;
			
		case w2:
			break;
			
		case res_w:
			break;
			
		case res_c:
			PORTC = 0x00;
			break;
			
		case w3:
			break;
			
		default:
			break;
	}
			
}

int main(void)
{
   DDRA = 0x00; PORTA = 0xFF;
   DDRC = 0xFF; PORTC = 0x07;
   state = init;

   while(1) {
	   tick();
   }
}

