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
	pid_Init(10, 10, 10, &this->pid);

	// Setup timer used to measure speed of the fan.
	TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10); // Set clock Prescaler to 64.
	TIMSK1 |= (1<<OCIE1A); // Enables Compare Interrupt for Timer.
	TCNT1 = 0; // Timer Default Value of 0.
	OCR1A = 62500; // Timer Compare value = 62500 so overflow happens at 500ms.

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
	// If the requested speed is between 0 < speed < 30 then clip speed at 30. And set an upper limit to 270.
	// Will need to invoke error handler for these cases.

	this->requestedSpeed = speed;
	// Keep setting the duty cycle until we get a currentSpeed that is within the bounds of the requested speed.
	//uint16_t dutyCycle = pid_Controller(this->requestedSpeed, this->currentSpeed, &this->pid);

	// Only need to reset integrator is the value overflows.
	pid_Reset_Integrator(&this->pid);
}

void SpeedController::measureSpeed() {
	// Measure the speed from the hall sensor and change the current speed on the controller.

	// Logic:
	// Timer times from 0 -> 0.5s
	// Hall sensor change adds 1 to speedCounter each time it changes state.
	// When timer has completed, see what the count is and /2 (or >>1) (due to hall sensor adding 1 on rising and falling edge).
	// speedCount * 2 = counts per 1 second = frequency.

	// Convert frequency to RPM/10 to get speed.
	// f(rpm) = f(Hz) * 60;
	this->currentSpeed = speedCount * 30;
}

uint8_t SpeedController::getRequestedSpeed() {
	return this->requestedSpeed;
}

uint8_t SpeedController::getCurrentSpeed() {
	return this->currentSpeed;
}
