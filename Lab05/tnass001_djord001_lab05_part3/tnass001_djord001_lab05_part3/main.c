/*
 * tnass001_djord001_lab05_part3.c
 *
 * Created: 4/19/2019 11:57:43 AM
 * Author : Disho
 */ 

#include <avr/io.h>

enum SM_FestivalStates{SM_Start, Seq,Inc, Wait}Current_State;

unsigned char temp_b, counter;

void FestivalLights_Tick(){
	
	switch(Current_State){ //State Transitions
		case SM_Start:
			Current_State = Seq;
		break;
		case Seq:
			Current_State = (~PINA & 0x01)? Inc: Seq;
		break;
		case Inc:
		Current_State = Wait;
		break;
		case Wait:
			Current_State = (~PINA & 0x01)? Wait: Seq;
		break;
		default:
		break;	
		
	}
	switch(Current_State){ //State Actions
		case SM_Start:
			counter = 0;
			temp_b = 0x00;
			break;
		case Seq:
		break;
		case Inc:
			counter = (counter == 3)? 0:counter + 1;
			break;
		case Wait:
			
			switch(counter){
				case 0:
				temp_b = 0x03;
				break;
				case 1:
				case 2:
				case 3:
				temp_b = temp_b << 1;
				default:
				break;
			}
		break;
		default:
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
		FestivalLights_Tick();
		PORTB = temp_b;
    }
}

