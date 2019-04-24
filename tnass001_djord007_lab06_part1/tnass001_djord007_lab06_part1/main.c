/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 6   Exercise 1
 *	Exercise Description: Alternate between LEDs [0-2] sequentially 
 */

#include <avr/io.h>
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

unsigned char timer;

enum LightStates{Init, LED0, LED1, LED2} LightState;

void tickLightSM() {
	switch(LightState) {
		case Init:
			LightState = LED0;
			break;
		
		case LED0:
			LightState = LED1;
			break;
			
		case LED1:
			LightState = LED2;
			break;
			
		case LED2:
			LightState = LED0;
			break;
			
		default:
			break;
	}
	
	switch(LightState) {
		case Init:
			break;
			
		case LED0:
			PORTB = 0x01;
			break;
			
		case LED1:
			PORTB = 0x02;
			break;
			
		case LED2:
			PORTB = 0x04;
			break;
			
		default:
			break;
	}
}
int main(void)
{
   DDRB = 0xFF; PORTB = 0x00;
   TimerSet(1000);
   TimerOn();
   timer = 0x00;
   LightState = Init;
 
    while (1) 
    {
		tickLightSM();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

