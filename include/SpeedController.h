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
#include "CommsController.h"

class SpeedController {
public:
	SpeedController();
	void setControllerPointers(volatile PWMController* pwmController, volatile ErrorHandler* errorHandler) volatile;

	// Sets the fans speed provided a speed.
	void setFanSpeed(uint8_t speed) volatile;

	// Measure the speed of the fan using the Hall sensor. Stores the speed in the currentSpeed.
	void measureSpeed() volatile;

	// Variables used to count speed.
	uint16_t speedCount;
	volatile uint8_t timerCount;

	// Speed variables.
	volatile uint8_t currentSpeed;
	uint8_t requestedSpeed;

	volatile bool bSpeedMeasured;

private:
	volatile PWMController* pwmController;
	volatile ErrorHandler* errorHandler;

	// PID controller.
	PID_DATA pid;
};

#endif /* SPEEDCONTROLLER_H_ */