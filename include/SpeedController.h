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
#include "PWMController.h"
#include "ErrorHandler.h"

class SpeedController {
public:
	SpeedController();
	void setControllerPointers(volatile PWMController* pwmController, volatile ErrorHandler* errorHandler) volatile;

	void run() volatile;

	// Sets the fans speed provided a speed.
	void setFanSpeed(uint8_t Speed) volatile;

	// Measure the speed of the fan using the Hall sensor. Stores the speed in the currentSpeed.
	void measureSpeed() volatile;

	// Variables used to count speed.
	uint16_t speedCount;
	volatile uint8_t timerCount;

	// Speed variables.
	volatile uint16_t currentSpeed;
	uint8_t requestedSpeed;
	volatile uint8_t lowerSpeed;
	volatile uint8_t upperSpeed;
	volatile uint8_t duty;

	volatile bool bSpeedMeasured;
private:
	volatile PWMController* pwmController;
	volatile ErrorHandler* errorHandler;
};

#endif /* SPEEDCONTROLLER_H_ */