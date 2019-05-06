/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 9 Exercise 1
*	Exercise Description:
*
*	A0 C4
*	A1 D4
*	A2 E4
*/

#include <avr/io.h>
#include <avr/interrupt.h>

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

enum States{Start, Wait, PlayC,PlayD,PlayE}STATE;
unsigned char a0,a1,a2;

void Tick(){

	switch(STATE){ //Transitions
		case Start:
			STATE = Wait;
		break;
		case Wait:
			if(a0 && !a1 && !a2){
				STATE = PlayC;
				PWM_on();
			}else if(!a0 && a1 && !a2){
				STATE = PlayD;
				PWM_on();
			}else if((!a0 && !a1 && a2)){
				STATE = PlayE;
				PWM_on();
			}else{
				STATE = Wait;
			}
		break;
		case PlayC:
			STATE = (a0 && !a1 && !a2)? PlayC: Wait;
		break;
		case PlayD:
			STATE = (!a0 && a1 && !a2)? PlayD: Wait;
		break;
		case PlayE:
			STATE = (!a0 && !a1 && a2)? PlayE: Wait;
		break;
		default:
			STATE = Start;
		break;

	}

	switch(STATE){ //Actions
		case Start:
			PWM_off();
		break;
		case Wait:
			PWM_off();
		break;
		case PlayC:
			set_PWM(261.63);
		break;
		case PlayD:
			set_PWM(293.66);
		break;
		case PlayE:
			set_PWM(329.63);
		break;
		default:
		break;

	}


}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	STATE = Start;

    while (1) 
    {
		a0 = ~PINA & 0x01;
		a1 = ~PINA & 0x02;
		a2 = ~PINA & 0x04;

		Tick();

    }
}

