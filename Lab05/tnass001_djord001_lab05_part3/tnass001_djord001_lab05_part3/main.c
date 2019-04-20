/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
 *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 5   Exercise 3
 *	Exercise Description: 
 *
 *  Create your own festive lights display with 6 LEDs connected to port PB5..PB0, lighting in some attractive sequence. 
 *  Pressing the button on PA0 changes the lights to the next configuration in the sequence.  
 *	Use a state machine (not synchronous) captured in C.
 */

#include <avr/io.h>

enum SM_FestivalStates{SM_Start, Seq1, Seq2, Seq3, Seq4, W1, W2, W3, W4 }Current_State;

unsigned char temp_b, a_out;

void FestivalLights_Tick(){
	
	switch(Current_State){ //State Transitions
		case SM_Start:
			Current_State = a_out? Seq1: SM_Start;
		break;
		case Seq1:
			Current_State = a_out? Seq1: W1;
		break;
		case W1:
			Current_State = a_out? Seq2: W1;
		break;
		case Seq2:
			Current_State = a_out? Seq2: W2;
		break;
		case W2:
			Current_State = a_out? Seq3: W2;
		break;
		case Seq3:
			Current_State = a_out? Seq3: W3;
		break;
		case W3:
			Current_State = a_out? Seq4: W3;
		break;
		case Seq4:
			Current_State = a_out? Seq4: W4;
		break;
		case W4:
			Current_State = a_out? Seq1: W4;
		break;
		default:
			Current_State = SM_Start;
		break;
	}		
	
	switch(Current_State){ //State Actions
		case SM_Start:
			temp_b = 0x00;
		break;
		case Seq1:
			temp_b = 0x28;
		break;
		case W1:
			temp_b = 0x28;
		break;
		case Seq2:
			temp_b = 0x14;
		break;
		case W2:
			temp_b = 0x14;
		break;
		case Seq3:
			temp_b = 0x0A;
		break;
		case W3:
			temp_b = 0x0A;
		break;
		case Seq4:
			temp_b = 0x05;
		break;
		case W4:
			temp_b = 0x05;
		break;
		default:
			temp_b = 0x00;
		break;
	}
		
}
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00;	PORTA = 0xFF;
	DDRB = 0xFF;	PORTB = 0x00;
	temp_b = 0x00;
	
    while (1) 
    {
		a_out = (~PINA) & 0x01;
		FestivalLights_Tick();
		PORTB = temp_b;
    }
}

