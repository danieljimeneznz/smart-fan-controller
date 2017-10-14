/*
 * CommsController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#ifndef COMMSCONTROLLER_H_
#define COMMSCONTROLLER_H_

#include "prototypes.h"

#define BAUD 9600 // Baud Rate
#define MYBRR 51 //(((F_CPU/(BAUD*16UL))) - 1)

#include <avr/io.h>
#include <stdint.h>

#include "SpeedController.h"
#include "PowerController.h"
#include "ErrorHandler.h"
#include "tinyjsonpp.h"

class CommsController {
public:
	CommsController(uint8_t ubrr);
	void transmit(uint8_t data) volatile;
	volatile bool bjsonComplete;
	void run() volatile;
	void setControllerPointers(volatile SpeedController* speedController, volatile PowerController* powerController, volatile ErrorHandler* errorHandler) volatile;

	tinyjsonpp* json;
	volatile uint8_t terminatingChar;
private:
	volatile SpeedController* speedController;
	volatile PowerController* powerController;
	volatile ErrorHandler* errorHandler;
};

#endif /* COMMSCONTROLLER_H_ */