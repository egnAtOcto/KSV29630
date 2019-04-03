#include <Arduino.h>

/*
 * Compressor.h
 *
 *  Created on: Nov 18, 2018
 *      Author: yam
 */

#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

#define COMP_STARTED 1
#define COMP_STOPPED 0

class Compressor {
private:
	int pin;
	byte state;
	unsigned long lastStopped;
	unsigned long lastStarted;
	int COMP_COOLINGTIME;
	int COMP_MINRUNTIME;

public:
	// pin refers to ESP pins
	Compressor(int pin);
	boolean start();
	boolean stop();
	void mqttCallback(char* topic, byte* payload, unsigned int length);
	void fridgeSwitchCallback();
};

#endif /* COMPRESSOR_H_ */
