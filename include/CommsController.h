/*
 * CommsController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef COMMSCONTROLLER_H_
#define COMMSCONTROLLER_H_

class CommsController {
public:
	CommsController(uint8_t ubrr);
	void transmit(uint8_t data);
	uint8_t recieve();
private:
	tinyjsonpp* json;
};

#endif /* COMMSCONTROLLER_H_ */