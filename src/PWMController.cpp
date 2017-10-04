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
	//set PA3 and PB2 as output (to drive the switches) DDRA
	DDRA |= (1<<DDA3);
	DDRB |= (1<<DDB2);
	//set PA0 as input (for hall-effect sensor)
	DDRA &= ~(1<<DDA0);

	//Set it to fast PWM
	TCCR0A |= (1<<WGM01)|(1<<WGM00);

	//Pre-scaling
	TCCR0B |= (1<<CS01);

	// clear OC0A on compare match with OCR0A
	TCCR0A |= (1 << COM0A1);
	TCCR0A |= (1 << COM0B1);

	TCNT0 = 0;	//reset counter register

	OCR0A = 0; //the Duty cycle n/255
	OCR0B = 0; //the Duty cycle n/255

	TOCPMSA0 &= ~((1<<TOCC7S1)|(1<<TOCC7S0));// route OC0A to PB2
	TOCPMSA0 &= ~((1<<TOCC2S1)|(1<<TOCC2S0));// route OC0B to PA3

	TIMSK0 |= (1<<TOIE0);	//Enable timer overflow interrupt

	// Set Hall Sensor to external pin change interrupt.
	PCMSK0 |= (1<<PCINT0);

	// Set Interrupt Sense control to trigger on any logic change.
	MCUCR |= (1<<ISC00);

	// Enable Pin Change Interrupt
	GIMSK |= (1<<PCIE0);
}

void PWMController::SetDutyCycle(uint8_t Duty){
	// 'kick' start the motor
	if(Duty > 0) {
		// Disable Pin Change Interrupt
		GIMSK &= ~(1<<PCIE0);
		
		OCR0A = 255;
		OCR0B = 255;
		for (uint8_t i = 0; i < 255; ++i) {
			if((PINA)&(1<<PINA0)) {
				TOCPMCOE &= ~ (1<<TOCC7OE);	//disable TOCC1 at PB2
				TOCPMCOE |= (1<<TOCC2OE);	//enable TOCC0 at PA3
				PORTB &= ~(1<<PORTB2);		//disable PB2
			} else {
				TOCPMCOE &= ~(1<<TOCC2OE);	//disable TOCC0 at PA3
				TOCPMCOE |= (1<<TOCC7OE);	//enable TOCC1 at PB2
				PORTA &= ~(1<<PORTA3);		//disable PA3
			}
		}

		// Re-Enable Pin Change Interrupt
		GIMSK |= (1<<PCIE0);
	}

	OCR0A = Duty;
	OCR0B = Duty;
	
	this->Duty = Duty;
}
