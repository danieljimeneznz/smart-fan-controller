/*
 * SpeedController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */

#include "SpeedController.h"

#define KP 0.65f
#define KI 0.15f

SpeedController::SpeedController() {
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
	this->lowerSpeed = 3;
	this->upperSpeed = 0;
}

void SpeedController::setControllerPointers(volatile PWMController* pwmController, volatile ErrorHandler* errorHandler) volatile {
	this->pwmController = pwmController;
	this->errorHandler = errorHandler;
}

void SpeedController::run() volatile {
	if(this->currentSpeed < (uint8_t)(lowerSpeed - dLowerSpeed - (dLowerSpeed/5)) || this->currentSpeed > (uint8_t)(upperSpeed + dUpperSpeed)) {
		setFanSpeed(requestedSpeed);
	}
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
		// Raise a reqTooLow error.
		errorHandler->reqTooLow = true;

		// Set minimum speed to lowest possible speed.
		speed = 30;
	} else {
		// Clear the reqTooLow error.
		errorHandler->reqTooLow = false;
	}

	// Change the Duty cycle until the speed is within acceptable limits. (+-5%)
	dLowerSpeed = (speed/20);
	dUpperSpeed = (speed/20);

	// Bound the upper speed limit (and lower speed limit for high speeds as max speed ~228rpm when blocked).
	if (speed > 240) {
		dLowerSpeed = 32;
		dUpperSpeed = 0;
	} else if(speed < 35) {
		dUpperSpeed = (speed/5);
	}

	lowerSpeed = speed - dLowerSpeed;
	upperSpeed = speed + dUpperSpeed;

	uint8_t lockedRotor = 0;
	float dutyCycle = (float)pwmController->Duty;
	this->duty = pwmController->Duty;
	int16_t error = 0;
	int16_t integral = 0;

	while ((this->currentSpeed < lowerSpeed || this->currentSpeed > upperSpeed)) {
		//Wait for a speed measurement to be taken.
		while(!this->bSpeedMeasured) {
			// Do Nothing.
		}

		// PID Controller based on: http://tutorial.cytron.io/2012/06/22/pid-for-embedded-design/
		error = (int16_t)speed - (int16_t)this->currentSpeed;
		integral = integral + error;

		dutyCycle = (KP * error) + (KI * integral);

		if (dutyCycle > 255) {
			dutyCycle = 255;
		} else if (dutyCycle <= 10) {
			// A speed request above zero was asked for so clip at a low acceptable duty cycle.
			dutyCycle = 10;
		}

		this->duty = (uint8_t)dutyCycle;

		// Locked rotor detection. i.e. we have asked for a Duty Cycle but have zero speed.
		if(duty > 0 && this->currentSpeed == 0) {
			++lockedRotor;
			if (lockedRotor > 1) {
				return;
			}
		}

		this->pwmController->SetDutyCycle(duty);

		this->bSpeedMeasured = false;
	}
}

void SpeedController::measureSpeed() volatile {
	// Check if we have counted to 1 seconds.
	if(this->timerCount < 2) {
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
	this->currentSpeed = speedCount/4 * 6;

	this->bSpeedMeasured = true;

	// Reset the speedCounter.
	this->speedCount = 0;
	this->timerCount = 0;
}
