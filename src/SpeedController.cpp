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
	pid_Init(95, 16, 0, &this->pid);

	// Setup timer used to measure speed of the fan.
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10); // Set clock Prescaler to 1024.
	TIMSK1 |= (1<<OCIE1A); // Enables Compare Interrupt for Timer.
	TCNT1 = 0; // Timer Default Value of 0.
	OCR1A = 23438; // Timer Compare value = 23438 so overflow happens at 3sec.

	// Initialize variables used by the counter to calculate speed.
	this->speedCount = 0;

	// Initialize speed variables.
	this->currentSpeed = 0;
	this->requestedSpeed = 0;
}

void SpeedController::setControllerPointers(PWMController* pwmController, ErrorHandler* errorHandler) {
	this->pwmController = pwmController;
	this->errorHandler = errorHandler;
}

void SpeedController::setFanSpeed(uint8_t speed) {
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
	uint8_t upperSpeed = speed + (speed/10);
	uint8_t transient = 0; // Used to perform the loop two more times until transients have settled down.
	do {
		// Wait for a speed measurement to be taken.
		while(this->speedCount > 0) {
			// Do nothing.
		}

		// Set a new duty cycle based on the new measured speed.
		uint16_t dutyCycle = pid_Controller(speed, this->currentSpeed, &this->pid);

		if(dutyCycle > 255) {
			dutyCycle = 255;
		}

		this->pwmController->SetDutyCycle(dutyCycle);

		// Only need to reset integrator if the value overflows.
		pid_Reset_Integrator(&this->pid);

		++transient;
	} while (speed < lowerSpeed || speed > upperSpeed || transient < 3);
}

void SpeedController::measureSpeed() {
	// Measure the speed from the hall sensor and change the current speed on the controller.

	// Logic:
	// Timer times from 0 -> 3s
	// Hall sensor change adds 1 to speedCounter each time it changes state.
	// When timer has completed, see what the count is and /2 (or >>1) (due to hall sensor adding 1 on rising and falling edge).
	// speedCount * 2 = counts per 1 second = frequency.
	// speedCount/3 as we are counting for 3 seconds.

	// Convert frequency to RPM/10 to get speed.
	// f(rpm) = f(Hz) * 60;
	this->currentSpeed = speedCount/2;

	// Reset the speedCounter.
	this->speedCount = 0;
}