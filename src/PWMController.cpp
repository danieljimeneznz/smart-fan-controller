/*
 * PWMController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 
#include <avr/io.h>
#include "PWMController.h"

PWMController::PWMController() {
	//set PA3 and PA7 as output (to drive the switches) DDRA
	DDRA |= (1<<DDA3)|(1<<DDA7);
	//set PA0 as input (for hall-effect sensor)
	DDRA &= ~(1<<DDA0);

	//Set it to fast PWM
	TCCR0A |= (1<<WGM01)|(1<<WGM00);

	//Pre-scaling
	TCCR0B |= (1<<CS01)|(1<<CS00);

	// clear OC0A on compare match with OCR0A
	TCCR0A |= (1 << COM0A1);
	TCCR0A |= (1 << COM0B1);

	TCNT0 = 0;	//reset counter register

	OCR0A = 100; //the Duty cycle n/255
	OCR0B = 100; //the Duty cycle n/255

	TOCPMSA0 &= ~((1<<TOCC6S1)|(1<<TOCC6S0));// route OC0A to PA7
	TOCPMSA0 &= ~((1<<TOCC2S1)|(1<<TOCC2S0));// route OC0B to PA3

	TIMSK0 |= (1<<TOIE0);	//Enable timer overflow interrupt
}
