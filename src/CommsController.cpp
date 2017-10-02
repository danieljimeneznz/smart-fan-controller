/*
 * CommsController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "CommsController.h"

CommsController::CommsController(uint8_t ubrr) {
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); // Enable Rx, Tx and Rx Complete Interrupt.
	UBRR0H = (ubrr>>8);
	UBRR0L = ubrr;
	// Select 8-bit data frame, double stop bit and no parity using UCSR0C (Default values are what we want).
	UCSR0C |= (1 << USBS0); // two stop bits

	// Create a new tinyJSONpp Object.
	//json = new tinyjsonpp(false, 255);
}

void CommsController::transmit(uint8_t data) {
	while (!(UCSR0A && (1 << UDRE0))); // Wait for empty transmit buffer.
	UDR0 = data;//sending data to TX buffer
	while (!(UCSR0A & (1<<TXC0))); // waits for Tx buffer to be empty
}

void CommsController::setControllerPointers(SpeedController* speedController, PowerController* powerController, ErrorHandler* errorHandler)
{
	this->speedController = speedController;
	this->powerController = powerController;
	this->errorHandler = errorHandler;
}
