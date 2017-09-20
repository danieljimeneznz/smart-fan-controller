/*
 * PowerController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "PowerController.h"

PowerController::PowerController() {
	return;
}

void PowerController::setControllerPointers(ErrorHandler* errorHandler) {
	this->errorHandler = errorHandler;
}

void PowerController::readVoltage() {
}