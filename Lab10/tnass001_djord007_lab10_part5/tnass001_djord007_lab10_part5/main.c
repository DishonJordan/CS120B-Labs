/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 10   Exercise 5
 *	Exercise Description: 
 *	
 *	Buttons are connected to PA0 and PA1. Output PORTB drives a bank of 4 LEDs. Pressing PA0 increments a binary number displayed on the bank of LEDs (stopping at 9). 
 *	Pressing PA1 decrements the binary number (stopping at 0). If both buttons are depressed (even if not initially simultaneously), the display resets to 0. If a 
 *	button is held, then the display continues to increment (or decrement) at a rate of once per second. However, if the button is held for 3 seconds, the 
 *	incrementing/decrementing occurs once per 400 ms. Use synchronous state machines captured in C.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

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

unsigned char b_out, counter, b_inc, b_dec;

enum C_States{Start, Wait, Inc, Dec, Reset}STATE;

void Tick(){
	switch(STATE){ //State Transitions
		case Start:
			STATE = Wait;
		break;
		case Wait:
			if(b_inc && b_dec){
				STATE = Reset;
			}else if(b_inc && ! b_dec){
				b_out = ( b_out < 9)? b_out + 1: 9;
				STATE = Inc;
			}else if(!b_inc && b_dec){
				b_out = ( b_out > 0)? b_out - 1: 0;
				STATE = Dec;
			}else{
				STATE = Wait;
			}
		break;
		case Inc:
			if(b_inc && b_dec){
				STATE = Reset;
			}else if(b_inc){
				STATE = Inc;
				counter++;
			}else{
				STATE = Wait;
			}
		break;
		case Dec:
		if(b_inc && b_dec){
			STATE = Reset;
			}else if(b_dec){
			STATE = Dec;
			counter++;
			}else{
			STATE = Wait;
		}			
		break;
		case Reset:
			STATE = ( !b_inc && !b_dec)? Wait : Reset;
		break;
		default:
			STATE = Start;
		break;
	}

	switch(STATE){ //State Actions
		case Start:			
			b_out = 0; counter = 0;
		break;
		case Wait:
			counter = 0;
		break;
		case Inc:
			if( counter < 30){
				if(counter % 10 == 0){
					b_out = ( b_out < 9)? b_out + 1: 9;
				}
			}else{
				if((counter - 30) % 4 == 0){
					b_out = ( b_out < 9)? b_out + 1: 9;
				}
			}

		break;
		case Dec:
			if( counter < 30){
				if(counter % 10 == 0){
					b_out = ( b_out > 0)? b_out - 1: 0;
				}
				}else{
				if((counter - 30) % 4 == 0){
					b_out = ( b_out > 0)? b_out - 1: 0;
				}
			}
		break;
		case Reset:
			b_out = 0;
		break;
		default:
		break;
	}


}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(10);
	TimerOn();
	STATE = Start;
	b_out = 0x00;
	counter = 0x00;

    while (1) 
    {
		b_inc = ~PINA & 0x01;
		b_dec = ~PINA & 0x02;

		Tick();
		PORTB = b_out & 0x0F;

		while (!TimerFlag);
		TimerFlag = 0;



    }
}

