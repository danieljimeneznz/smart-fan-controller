/*
 * SpeedController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef SPEEDCONTROLLER_H_
#define SPEEDCONTROLLER_H_

#include "prototypes.h"

#include <stdint.h>
extern "C" {
	#include "pid.h"
}

#include "PWMController.h"
#include "ErrorHandler.h"

class SpeedController {
public:
	SpeedController();
	void setControllerPointers(PWMController* pwmController, ErrorHandler* errorHandler);

	// Sets the fans speed provided a speed.
	void setFanSpeed(uint8_t speed);

	// Measure the speed of the fan using the Hall sensor. Stores the speed in the currentSpeed.
	void measureSpeed();

	// Variables used to count speed.
	uint16_t speedCount;

	// Speed variables.
	uint8_t currentSpeed;
	uint8_t requestedSpeed;

private:
	PWMController* pwmController;
	ErrorHandler* errorHandler;

	// PID controller.
	PID_DATA pid;
};

#endif /* SPEEDCONTROLLER_H_ */