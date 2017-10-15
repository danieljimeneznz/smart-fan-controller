/*
 * ErrorHandler.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() {

	// Set class variables.
	this->reqTooLow = false;
	this->blockedDuct = false;
	this->lockedRotor = false;

	// Setup timer used to rebroadcast error messages.
	TCCR2B |= (0<<CS22)|(1<<CS21)|(1<<CS20); // Set clock Prescaler to 64.
	TIMSK2 |= (1<<OCIE2A); // Enables Compare Interrupt for Timer.
	TCNT2 = 0; // Timer Default Value of 0.
	OCR2A = 62500; // Timer Compare value = 62500 so overflow happens at ~500ms.

	this->timerCount = 0;
}

void ErrorHandler::clearErrors() volatile {
	this->reqTooLow = false;
	this->blockedDuct = false;
	this->lockedRotor = false;
}

void ErrorHandler::run() volatile {
	// This function is run every 1 second and will retransmit any errors on the comms controller if it needs to.
	// It shares the timer that is used to measure the speed.
	// It will also check for a blockedDuct and will ensure the fan is running at the correct speed.
	
	if(this->timerCount < 2) {
		return;
	}

	// Locked rotor detection. i.e. we have asked for a Duty Cycle but have zero speed.
	if(speedController->duty > 0 && speedController->currentSpeed == 0) {
		this->lockedRotor = true;
	} else {
		this->lockedRotor = false;
	}

	// Special case not to rebroadcast errors for zero speed request.
	if(speedController->requestedSpeed == 0) {
		this->lockedRotor = false;
		this->reqTooLow = false;
	}

	if (this->reqTooLow || this->blockedDuct || this->lockedRotor) {
		commsController->bReTransmit = true;
	}

	// Reset the timerCount.
	this->timerCount = 0;
}

bool ErrorHandler::checkForErrors() volatile {
	if (this->reqTooLow && !this->blockedDuct && !this->lockedRotor) {
		// Only a request too low error.
		this->errorEEPROMStart = 38;
		this->errorEEPROMLength = 14;
	} else if (!this->reqTooLow && this->blockedDuct && !this->lockedRotor) {
		// Only a blockedDuct.
		this->errorEEPROMStart = 52;
		this->errorEEPROMLength = 16;
	} else if (!this->reqTooLow && !this->blockedDuct && this->lockedRotor) {
		// Only a locked rotor.
		this->errorEEPROMStart = 68;
		this->errorEEPROMLength = 16;
	} else if (this->reqTooLow && this->blockedDuct && !this->lockedRotor) {
		// Request too low and blocked duct.
		this->errorEEPROMStart = 84;
		this->errorEEPROMLength = 28;
	} else if (!this->reqTooLow && this->blockedDuct && this->lockedRotor) {
		// Blocked duct and locked rotor.
		this->errorEEPROMStart = 140;
		this->errorEEPROMLength = 30;
	} else if (this->reqTooLow && !this->blockedDuct && this->lockedRotor) {
		// Request too low and locked rotor.
		this->errorEEPROMStart = 112;
		this->errorEEPROMLength = 28;
	} else if (this->reqTooLow && this->blockedDuct && this->lockedRotor) {
		// All three errors.
		this->errorEEPROMStart = 170;
		this->errorEEPROMLength = 42;
	}

	// An error is present so return this.
	return this->reqTooLow || this->blockedDuct || this->lockedRotor;
}

void ErrorHandler::setControllerPointers(volatile CommsController* commsController, volatile SpeedController* speedController) volatile {
	this->commsController = commsController;
	this->speedController = speedController;
}
