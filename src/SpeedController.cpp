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
	uint16_t dutyCycle = pid_Controller(this->requestedSpeed, this->currentSpeed, &this->pid);
	measureSpeed();

	// Only need to reset integrator is the value overflows.
	pid_Reset_Integrator(&this->pid);
}

void SpeedController::measureSpeed() {
	// Measure the speed from the hall sensor and change the current speed on the controller.
	currentSpeed = 100;
}

uint8_t SpeedController::getRequestedSpeed() {
	return this->requestedSpeed;
}

uint8_t SpeedController::getCurrentSpeed() {
	return this->currentSpeed;
}
