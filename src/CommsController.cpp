/*
 * CommsController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "CommsController.h"

ISR(USART0_RX_vect) {// There are two interrupts available,
	//one is USART0_RXC_vect (waits for completion) and USART0_RXs_vect (waits for start of incoming)
	//but atmel in only accepting USART0_RX_vect which i cant find in the data sheet
	while (!(UCSR0A & (1<<RXC0)))//loop waits for completion of incoming
	
	//commsController->json->parse(UDR0);

	if (UDR0 == 'd'){
		commsController->jsonComplete = true;
		PORTA |= (1 << PORTA7)
	}
}

CommsController::CommsController(uint8_t ubrr) {
	UCSR1B = (1<<RXEN1) // Enable USART  receiver
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	// Enable Transmitter
	UCSR0B = (1 << TXEN0); // 0b00001100
	// Select 8-bit data frame, single stop bit and no parity using UCSR0C (Default values are what we want).

	// Set PA7 as Output.
	DDRA |= (1<<DDA7);

	// Create a new tinyJSONpp Object.
	//json = new tinyjsonpp(false, 255);
}

void CommsController::transmit(uint8_t data) {
	// Wait for empty transmit buffer.
	while (!(UCSR0A && (1 << UDRE0)) == 0);
	UDR0 = data;
	// After transmitting wait for a period of time before allowing next transmit.
	_delay_us(1500);
}

void CommsController::setControllerPointers(SpeedController* speedController, PowerController* powerController, ErrorHandler* errorHandler)
{
	this->speedController = speedController;
	this->powerController = powerController;
	this->errorHandler = errorHandler;
}
