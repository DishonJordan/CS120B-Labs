/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 4  Exercise 3 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum Lock_States{ Lock_Start, Lock_Combo1, Lock_Combo2, Lock_Unlock}Current_State;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned char lock_status = 0x01;
	unsigned char x_button = 0x00;
	unsigned char y_button = 0x00;
	unsigned char pound_button = 0x00;
	unsigned char inside_button = 0x00;
	Current_State = Lock_Start;
    /* Replace with your application code */
    while (1) 
    {
		x_button = PINA & 0x01;
		y_button = PINA & 0x02 >> 1;
		pound_button = PINA & 0x04 >> 2;
		inside_button = PINA & 0x08 >> 3;

		switch(Current_State){//Transition Controller
			case Lock_Start:
				Current_State =  (pound_button && !x_button && !y_button)? Lock_Combo1 : Lock_Start;
				break;
			case Lock_Combo1:
				if(x_button || y_button){
					Current_State = Lock_Start;
				}else if(!pound_button){
					Current_State = Lock_Combo2;
				}else{
					Current_State = Lock_Combo1;
				}

			break;
			case Lock_Combo2:
				if(pound_button || x_button){
					Current_State = Lock_Start;
				}else if(y_button){
					Current_State = Lock_Unlock;
				}else{
					Current_State = Lock_Combo2;
				}


			break;
			case Lock_Unlock:
				if(inside_button){
					Current_State = Lock_Start;
				}else{
					Current_State = Lock_Unlock;
				}
			break;
			default:
			break;
		}

		switch(Current_State){//State Actions Controller
			case Lock_Start:
				lock_status = 0x01;
				break;
			case Lock_Combo1:
				lock_status = 0x01;
				break;
			case Lock_Combo2:
				lock_status = 0x01;
				break;
			case Lock_Unlock:
				lock_status = 0x00;
				break;
			default:
			break;
		}

		PORTB = lock_status;
    }
}

