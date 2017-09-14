/*
 * CommsController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#ifndef COMMSCONTROLLER_H_
#define COMMSCONTROLLER_H_

#define F_CPU 8000000UL // Clock Speed

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "tinyjsonpp.h"

class CommsController {
public:
	CommsController(uint8_t ubrr);
	void transmit(uint8_t data);
	void recieve();
private:
	tinyjsonpp* json;
};

#endif /* COMMSCONTROLLER_H_ */