/*
 * Smart-Fan-Controller.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "prototypes.h"

#include "CommsController.h"
#include "PowerController.h"
#include "PWMController.h"
#include "SpeedController.h"
#include "ErrorHandler.h"

#include "tinyjsonpp.h"

// Pointers used by ISR.
CommsController* commsController;
PowerController* powerController;
PWMController* pwmController;
SpeedController* speedController;
ErrorHandler* errorHandler;

ISR(ADC_vect) {
	//powerController->readVoltage(11);
}

ISR(USART0_RX_vect) {// There are two interrupts available,
	//one is USART0_RXC_vect (waits for completion) and USART0_RXs_vect (waits for start of incoming)
	//but atmel in only accepting USART0_RX_vect which i cant find in the data sheet
	//while (!(UCSR0A & (1<<RXC0)))//loop waits for completion of incoming
	
	uint8_t data = UDR0;
	if (data == 'd'){
		commsController->jsonComplete = true;
		commsController->transmit(speedController->currentSpeed);
	}

	if (data > 30) {
		commsController->json->addCharToJSONString(data);
	}
}

ISR(TIMER0_OVF_vect){
	PORTA |= (1<<PORTA1);
	PORTA |= (1<<PORTA2);
}

ISR(TIMER1_COMPA_vect){
	++speedController->timerCount;
	speedController->measureSpeed();
}

// ISR for hall sensor.
ISR(PCINT0_vect) {
	if((PINA)&(1<<PINA0)) {
		TOCPMCOE &= ~ (1<<TOCC7OE);	//disable TOCC1 at PB2
		TOCPMCOE |= (1<<TOCC2OE);	//enable TOCC0 at PA3
		PORTB &= ~(1<<PORTB2);		//disable PB2
	} else {
		TOCPMCOE &= ~(1<<TOCC2OE);	//disable TOCC0 at PA3
		TOCPMCOE |= (1<<TOCC7OE);	//enable TOCC1 at PB2
		PORTA &= ~(1<<PORTA3);		//disable PA3
	}

	// Increment the speedCounter.
	++speedController->speedCount;
}

int main(void)
{
	// Instantiate objects in stack.
	CommsController commsC(MYBRR);
	PowerController powerC;
	PWMController pwmC;
	SpeedController speedC;
	ErrorHandler errorH;

	// Set relationship between objects
	commsC.setControllerPointers(&speedC, &powerC, &errorH);
	powerC.setControllerPointers(&errorH);
	speedC.setControllerPointers(&pwmC, &errorH);
	errorH.setControllerPointers(&commsC);

	// Set ISR pointers
	commsController = &commsC;
	powerController = &powerC;
	pwmController = &pwmC;
	speedController = &speedC;
	errorHandler = &errorH;

	// Enable Interrupts
	sei(); // Set global interrupt enable.
	speedController->setFanSpeed(20);

    while (1)                  
    {
		//commsController->transmit('b');
		commsController->run();
	}
}
