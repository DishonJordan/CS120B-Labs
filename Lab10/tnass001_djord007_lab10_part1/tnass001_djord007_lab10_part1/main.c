/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 10 Exercise 1
*	Exercise Description:
*	Connect LEDs to PB0, PB1, PB2, and PB3. In one state machine (Three LEDs), output to a shared variable (threeLEDs) the following behavior: 
*	set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second each. In a second state machine (Blinking LED), output to a 
*	shared variable (bilnkingLED) the following behavior: set bit 3 to 1 for 1 second, then 0 for 1 second. In a third state machine (Combine LEDs), 
*	combine both shared variables and output to the PORTB. Note: only one SM is writing to outputs. Do this for the rest of the quarter.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned char blinkingLEDs = 0x00;
unsigned char threeLEDs = 0x00;
unsigned char b_out = 0x00;

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


enum SM_BlinkLED{BlinkLED_ON, BlinkLED_OFF}BLINK_STATE;
enum SM_ThreeLED{ThreeLED_ONE, ThreeLED_TWO, ThreeLED_THREE}THREE_STATE;

void BlinkLED_Tick(){
	switch(BLINK_STATE){
		case BlinkLED_OFF: BLINK_STATE = BlinkLED_ON; 
		break;
		case BlinkLED_ON:  BLINK_STATE = BlinkLED_OFF; 
		break;
		default: BLINK_STATE = BlinkLED_OFF; 
		break;
	}
	switch(BLINK_STATE){
		case BlinkLED_OFF: blinkingLEDs = 0x00;
		break;
		case BlinkLED_ON:  blinkingLEDs = 0x08;
		break;
		default:;
		break;
	}

}
void ThreeLED_Tick(){
	switch(THREE_STATE){
	case ThreeLED_ONE: 
		THREE_STATE = ThreeLED_TWO;
	break;
	case ThreeLED_TWO:
		THREE_STATE = ThreeLED_THREE;
	break;
	case ThreeLED_THREE:
		THREE_STATE = ThreeLED_ONE;
	break;
	default:
		THREE_STATE = ThreeLED_ONE;
	break;
	}
	switch(THREE_STATE){
	case ThreeLED_ONE:
		threeLEDs = 0x01;
	break;
	case ThreeLED_TWO:
		threeLEDs = 0x02;
	break;
	case ThreeLED_THREE:
		threeLEDs = 0x04;
	break;
	default:
	break;
	}

}

void CombineLEDs_Tick(){

	BlinkLED_Tick();
	ThreeLED_Tick();
	b_out =  blinkingLEDs | threeLEDs;

}
int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(100);
	TimerOn();
	blinkingLEDs = 0x00;
	threeLEDs = 0x00;
	BLINK_STATE = BlinkLED_OFF;
	THREE_STATE = ThreeLED_ONE;

    while (1) 
    {
		CombineLEDs_Tick();
		PORTB = b_out;
		while(!TimerFlag);
		TimerFlag = 0;
		
    }
}

