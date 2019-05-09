/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 9 Exercise 2
*	Exercise Description:
*
*	A0 Start/Stop
*	A1 Inc
*	A2 Dec

*/

#include <avr/io.h>

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


enum States{Start,WaitForRelease,Play,Stop,Inc,Dec,WaitStop,WaitPlay}STATE;

unsigned char a0, a1, a2, current_note;

double note[8] = {261.63,293.66,329.63,349.23,392.00,440.00,493.88,523.25};


void Tick(){
	switch(STATE){//Transitions
		case Start:
			STATE = a0? WaitForRelease: Start;
		break;
		case WaitForRelease:
			STATE = a0? WaitForRelease: Play;
		break;
		case Play:
			if(!a0 && !a1 && !a2){
				STATE = Play;
			}else if(a0 && !a1 && !a2){// Play/Pause button pressed
				STATE = WaitStop;
			}else if(!a0 && a1 && !a2){// Increment button pressed
				current_note = (current_note == 7)? 7: current_note + 1;
				STATE = Inc;
				
			}else if(!a0 && !a1 && a2){// Decrement button pressed
				current_note = (current_note == 0)? 0: current_note - 1;
				STATE = Dec;
				
			}else{ //Two are pressed at the same time so do nothing
				STATE = Play;
			}
		break;
		case Stop:
			STATE = a0? WaitPlay: Stop;
		break;
		case Inc:
			STATE = a1? Inc : Play;
		break;
		case Dec:
			STATE = a2? Dec : Play;
		break;
		case WaitStop:
			STATE = a0? WaitStop:Stop;
		break;
		case WaitPlay:
			STATE = a0? WaitPlay:Play;
		break;
		default:
			STATE = Start;
		break;
	}

	switch(STATE){//Actions
		case Start:
			current_note = 0;
			PWM_on();
		break;
		case WaitForRelease:
		break;
		case Play:
			set_PWM(note[current_note]);
		break;
		case Stop:
			set_PWM(0);
		break;
		case Inc:
			set_PWM(note[current_note]);
		break;
		case Dec:
			set_PWM(note[current_note]);
		break;
		case WaitStop:
		break;
		case WaitPlay:
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
	current_note = 0;

	while(1){
	a0 = ~PINA & 0x01;
	a1 = ~PINA & 0x02;
	a2 = ~PINA & 0x04;
	
	Tick();
	}



}

