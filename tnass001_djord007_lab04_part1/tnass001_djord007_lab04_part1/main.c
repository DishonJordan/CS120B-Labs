/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
	
enum States {LEDStart, LED1, LED2} LEDState ;
		
void TickFct_Latch() {
		switch(LEDState) {
			case LEDStart:
			LEDState = LED1;
			break;

			case LED1:
			if ((PINA & 0x01) == 0x01) {
				LEDState = LED2;
			}
			else if ((PINA & 0x00) == 0x00) {
				LEDState = LED1;
			}
			break;
			
			case LED2:
			if ((PINA & 0x01) == 0x01) {
				LEDState = LED2;
			}
			else if ((PINA & 0x00) == 0x00) {
				LEDState = LED1;
			}
			break;
			
			default:
			LEDState = LEDStart;
			break;
			
		}
		
		switch(LEDState) {
			case LED1:
			PORTB = 0x01;
			break;
			
			case LED2:
			PORTB = 0x02;
			break;
			
			default:
			break;
		}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	LEDState = LEDStart;
	
	while(1) {
		TickFct_Latch();
	}
}

