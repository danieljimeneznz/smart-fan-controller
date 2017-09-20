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

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "SpeedController.h"
#include "PowerController.h"
#include "ErrorHandler.h"
#include "tinyjsonpp.h"

class CommsController {
public:
	CommsController(uint8_t ubrr);
	void transmit(uint8_t data);
	void recieve();
	void setControllerPointers(SpeedController* speedController, PowerController* powerController, ErrorHandler* errorHandler);
private:
	SpeedController* speedController;
	PowerController* powerController;
	ErrorHandler* errorHandler;
	//tinyjsonpp* json;
};

#endif /* COMMSCONTROLLER_H_ */