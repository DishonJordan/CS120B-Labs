/*	
 *  Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
 *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 7   Exercise 2
 *	Exercise Description: (Challenge) Extend the earlier light game to maintain a score on the LCD display. 
 *	The initial score is 5. Each time the user presses the button at the right time, the score increments. 
 *	Each time the user fails, the score decrements. When reaching 9, show victory somehow. 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


unsigned char a_input, temp_b, timer, score;

enum Light_States{Init, WaitStartPress, WaitStartRelease, Light, WaitLightRelease, Win}LIGHT_STATE;

void Tick_LightSM(){

	//State Transitions
	switch(LIGHT_STATE){
		case Init:
			LIGHT_STATE = WaitStartPress;
		break;
		case WaitStartPress:
			LIGHT_STATE = a_input? WaitStartRelease : WaitStartPress;
		break;
		case WaitStartRelease:
			LIGHT_STATE = a_input? WaitStartRelease : Light;
		break;
		case Light:
			LIGHT_STATE = a_input? WaitLightRelease : Light;
			if(a_input){
				LIGHT_STATE = WaitLightRelease;
				if(temp_b = 0x02){
					score++;
					}else if(score > 0 ){
					score--;
					}else{
					//keep score at 0
				}
			}else{
				LIGHT_STATE = Light;
			}	
		break;
		case WaitLightRelease:
			if(score == 9){
				LIGHT_STATE = Win;
			}else if(a_input){
				LIGHT_STATE = WaitLightRelease;
			}else{
				LIGHT_STATE = WaitStartPress;
			}
		break;
		case Win:
			LIGHT_STATE = Win;
		break;
		default:
		break;
	}

	//State Actions
	switch(LIGHT_STATE){
		case Init:
			temp_b = 0x00;
			LCD_DisplayString(1, "5");
		break;
		case WaitStartPress:
		break;
		case WaitStartRelease:
			temp_b = 0x01;
			timer = 0x00;
		break;
		case Light:
			timer++;
			if(timer % 3 == 0)
			temp_b = (temp_b == 0x04)? 0x01: temp_b << 1;
		break;
		case WaitLightRelease:
			LCD_DisplayString(1, score);
		break;
		case Win:
			LCD_DisplayString(1, "Victory");
		break;
		default:
		break;
	}


}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	TimerSet(100);
	TimerOn();

	temp_b = 0x00; a_input = 0x00; timer = 0x00; score = 5;
	LIGHT_STATE = Init;
	LCD_init();

	while (1)
	{
		a_input = ~PINA & 0x01;
		Tick_LightSM();
		PORTB = temp_b & 0x07;

		while(!TimerFlag);
		TimerFlag = 0;

	}
}

