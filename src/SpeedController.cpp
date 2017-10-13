/*
 * SpeedController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */

#include "SpeedController.h"

SpeedController::SpeedController() {
	// Initialize the PID controller.
	pid_Init(60, 0, 0, &this->pid);
	this->bSpeedMeasured = false;

	// Setup timer used to measure speed of the fan.
	TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10); // Set clock Prescaler to 64.
	TIMSK1 |= (1<<OCIE1A); // Enables Compare Interrupt for Timer.
	TCNT1 = 0; // Timer Default Value of 0.
	OCR1A = 62500; // Timer Compare value = 62500 so overflow happens at 500ms.

	// Initialize variables used by the counter to calculate speed.
	this->speedCount = 0;
	this->timerCount = 0;

	// Initialize speed variables.
	this->currentSpeed = 0;
	this->requestedSpeed = 0;
}

void SpeedController::setControllerPointers(volatile PWMController* pwmController, volatile ErrorHandler* errorHandler) volatile {
	this->pwmController = pwmController;
	this->errorHandler = errorHandler;
}

void SpeedController::setFanSpeed(uint8_t speed) volatile {
	this->requestedSpeed = speed;

	// If zero speed is requested then set the duty cycle to zero.
	if(speed == 0) {
		pwmController->SetDutyCycle(0);
		return;
	}

	// If the requested speed is between 0 < speed < 30 then clip speed at 30.
	if(speed < 30) {
		// Raise a speed reqTooLow error.
		errorHandler->reqTooLow = true;

		// Set minimum speed to lowest possible speed.
		speed = 30;
	}

	// Change the Duty cycle until the speed is within acceptable limits. (+-10%)
	uint8_t lowerSpeed = speed - (speed/10);
	uint8_t upperSpeed;

	// Bound the upper speed limit.
	if (speed > 230) {
		upperSpeed = 255;
	} else {
		upperSpeed = speed + (speed/10);
	
	}
	//this->pwmController->SetDutyCycle(speed);

	while (this->currentSpeed < lowerSpeed || this->currentSpeed > upperSpeed) {
		//Wait for a speed measurement to be taken.
		while(!bSpeedMeasured) {
			// Do Nothing.
		}

		// Set a new duty cycle based on the new measured speed.
		uint16_t dutyCycle = pid_Controller(speed, this->currentSpeed, &this->pid);

		if(dutyCycle > 255) {
			dutyCycle = 255;
		}

		this->pwmController->SetDutyCycle(dutyCycle);

		// Only need to reset integrator if the value overflows.
		pid_Reset_Integrator(&this->pid);

		bSpeedMeasured = false;
	};
}

void SpeedController::measureSpeed() volatile {
	// Check if we have counted to 3 seconds.
	if(this->timerCount < 6) {
		return;
	}

	// Measure the speed from the hall sensor and change the current speed on the controller.

	// Logic:
	// Timer times from 0 -> 1s
	// Hall sensor change adds 1 to speedCounter each time it changes state. (The hall sensor triggers 4 times per revolution).
	// When timer has completed, see what the count is and /4 (or >>2) (due to hall sensor adding 1 on rising and falling edge).
	// speedCount/1 as we are counting for 1 seconds.

	// Convert frequency to RPM/10 to get speed.
	// f(rpm) = f(Hz) * 60;
	this->currentSpeed = speedCount/4 * 2;

	this->bSpeedMeasured = true;

	// Reset the speedCounter.
	this->speedCount = 0;
	this->timerCount = 0;
}
