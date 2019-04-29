/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 7 Exercise 1
*	Exercise Description:	
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>	
#include "io.c"
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum States {init, wait, inc, dec, reset, incWait, decWait, resWait} state;
unsigned char cValue = 0, timer = 0x00;

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
			cValue = 0;
			LCD_Cursor(1);
			LCD_WriteData(cValue + '0');
		break;
		
		case wait:
		timer = 0;
		break;
		
		case inc:
			if (cValue < 9) {
				cValue += 1;
				LCD_Cursor(1);
				LCD_WriteData(cValue + '0');
			}
		break;
		
		case dec:
		if (cValue > 0) {
			cValue -= 1;
			LCD_Cursor(1);
			LCD_WriteData(cValue + '0');
		}
		break;
		
		case reset:
			cValue = 0;
			LCD_Cursor(1);
			LCD_WriteData(cValue + '0');
		break;
		
		case incWait:
		timer++;
		if (cValue < 9 && (timer % 10 == 0)) {
			cValue += 1;
			LCD_Cursor(1);
			LCD_WriteData(cValue + '0');
			
		}
		break;
		
		case decWait:
		timer++;
			if (cValue > 0 && (timer % 10 == 0)) {
				cValue -= 1;
				LCD_Cursor(1);
				LCD_WriteData(cValue + '0');
				
			}
		break;
		
		case resWait:
		timer = 0;
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	TimerSet(100);
	TimerOn();
	
	LCD_init();
	LCD_Cursor(1);
	
	state = init;
	LCD_DisplayString(1, "0");
	while(1) {
		tick();
		
		while(!TimerFlag);
			TimerFlag = 0;
	}
}

