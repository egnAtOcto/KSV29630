/*
 * Compressor.cpp
 *
 *  Created on: Nov 18, 2018
 *      Author: yam
 */

#include "Compressor.h"
#include "pins.h"
#include "Config.h"

Compressor::Compressor(int pin) {
	this->pin = pin;

	// Start with compressor off
	pinMode(this->pin, OUTPUT);
	digitalWrite(this->pin, LOW);
	lastStopped = millis();
	state = COMP_STOPPED;
	COMP_COOLINGTIME = Config::getInstance()->getInt("compTimeBeforeRestart");
	COMP_MINRUNTIME = Config::getInstance()->getInt("compMinRunTime");
}

boolean Compressor::start() {
	if (state == COMP_STARTED)
		return true;
	if (millis() - lastStopped < COMP_COOLINGTIME)
		return false;
	digitalWrite(this->pin, HIGH);
	state = COMP_STARTED;
	lastStarted = millis();
	return true;
}

void Compressor::mqttCallback(char* topic, byte* payload, unsigned int length) {
}
void Compressor::fridgeSwitchCallback() {
}

boolean Compressor::stop() {
	if (state == COMP_STOPPED)
		return true;;
	if (millis() - lastStarted < COMP_MINRUNTIME)
		return false;
	digitalWrite(this->pin, LOW);
	state = COMP_STOPPED;
	lastStopped = millis();
	return true;
}
