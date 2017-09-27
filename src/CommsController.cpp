/*
 * CommsController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "CommsController.h"

CommsController::CommsController(uint8_t ubrr) {
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable USART  receiver
	UBRR0H = (ubrr>>8);
	UBRR0L = ubrr;
	// Select 8-bit data frame, single stop bit and no parity using UCSR0C (Default values are what we want).
	UCSR0C = (1 << USBS0); // two stop bits
	// Set PA7 as Output.
	DDRA |= (1<<DDA7);

	// Create a new tinyJSONpp Object.
	//json = new tinyjsonpp(false, 255);
}

void CommsController::transmit(unsigned char data) {
		//Enable Transmit
	// Wait for empty transmit buffer.
	while (!(UCSR0A && (1 << UDRE0)));
	UDR0 = data;
	// After transmitting wait for buffer to be empty before allowing next transmit.
}

void CommsController::setControllerPointers(SpeedController* speedController, PowerController* powerController, ErrorHandler* errorHandler)
{
	this->speedController = speedController;
	this->powerController = powerController;
	this->errorHandler = errorHandler;
}
