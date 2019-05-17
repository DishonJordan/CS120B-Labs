/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 11 Exercise 1
*	Exercise Description:
*	Modify the keypad code to be in an SM task. Then, modify the keypad SM to utilize the simple task 
*	scheduler format (refer to PES Chp 7). All code from here on out should use the task scheduler. 
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

unsigned char GetKeypadKey() {

	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1�s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1�s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }

	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1�s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	// Check keys in col 4
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }

	return('\0'); // default value

}


//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum SM1_States { GetKey };

// Monitors button connected to PA0. 
// When button is pressed, shared variable "pause" is toggled.
int SMTick1(int state) {
	unsigned char x;

	switch (state){
		case GetKey:
			state = GetKey;
		break;
		default:
			state = GetKey;
	}

	switch (state){
			case GetKey:
				x = GetKeypadKey();
				switch (x) {
					case '\0': PORTB = 0x1F; break; // All 5 LEDs on
					case '1': PORTB = 0x01; break; // hex equivalent
					case '2': PORTB = 0x02; break;
					case '3': PORTB = 0x03; break;
					case '4': PORTB = 0x04; break;
					case '5': PORTB = 0x05; break;
					case '6': PORTB = 0x06; break;
					case '7': PORTB = 0x07; break;
					case '8': PORTB = 0x08; break;
					case '9': PORTB = 0x09; break;
					case 'A': PORTB = 0x0A; break;
					case 'B': PORTB = 0x0B; break;
					case 'C': PORTB = 0x0C; break;
					case 'D': PORTB = 0x0D; break;
					case '*': PORTB = 0x0E; break;
					case '0': PORTB = 0x00; break;
					case '#': PORTB = 0x0F; break;
					default: PORTB = 0x1B; break; // Should never occur. Middle LED off.
				}
			break;
			default:
			break;
		}
	return state;
}

// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
// Set Data Direction Registers
DDRB = 0xFF; PORTB = 0x00;
DDRC = 0xF0; PORTC = 0x0F;
// . . . etc

// Period for the tasks
unsigned long int SMTick1_calc = 50;

//Calculating GCD
unsigned long int tmpGCD = 1;

//Greatest common divisor for all tasks or smallest time unit for tasks.
unsigned long int GCD = tmpGCD;

//Recalculate GCD periods for scheduler
unsigned long int SMTick1_period = SMTick1_calc/GCD;

//Declare an array of tasks 
static task task1;
task *tasks[] = { &task1};
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = -1;//Task initial state.
task1.period = SMTick1_period;//Task Period.
task1.elapsedTime = SMTick1_period;//Task current elapsed time.
task1.TickFct = &SMTick1;//Function pointer for the tick.

// Set the timer and turn it on
TimerSet(GCD);
TimerOn();

unsigned short i; // Scheduler for-loop iterator
while(1) {
	// Scheduler code
	for ( i = 0; i < numTasks; i++ ) {
		// Task is ready to tick
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			// Setting next state for task
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			// Reset the elapsed time for next tick.
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag);
	TimerFlag = 0;
}

// Error: Program should not exit!
return 0;
}
