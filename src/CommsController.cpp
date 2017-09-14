/*
 * CommsController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "..\include\CommsController.h"
#include "tinyjsonpp.h"

CommsController::CommsController(uint8_t ubrr) {
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	// Enable Transmitter
	UCSR0B = (1 << TXEN0); // 0b00001100
	// Select 8-bit data frame, single stop bit and no parity using UCSR0C (Default values are what we want).

	// Create a new tinyJSONpp Object.
	json = new tinyjsonpp(false, 255);
}

CommsController::transmit(uint8_t data) {
	// Wait for empty transmit buffer.
	while ((UCSR0A && (1 << UDRE0)) == 0);
	UDR0 = data;
	// After transmitting wait for a period of time before allowing next transmit.
	_delay_us(1500);
}

// Receive the data from the master
uint8_t CommsController::recieve() {
	json->parse('x');
}
