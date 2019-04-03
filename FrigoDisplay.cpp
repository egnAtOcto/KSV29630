/*
 * FrigoDisplay.cpp
 *
 *  Created on: Nov 20, 2018
 *      Author: yam
 */

#include "FrigoDisplay.h"
#include <Wire.h>

boolean blink;

void FrigoDisplay::refresh() {
	if (blink) {
		Wire.beginTransmission(PCF8574);
		Wire.write(0x0);
		byte error = Wire.endTransmission();
		delay(500);
		Wire.beginTransmission(PCF8574);
		Wire.write(0xff);
		error = Wire.endTransmission();
		blink = false;
	}
	if (currentDMode == DMODE_SET_TEMP
			&& millis() - previousDModeSetTemp > DMODE_SET_TEMP_TIMEOUT)
		currentDMode = DMODE_DISPLAY_TEMP;
	if (currentDMode == DMODE_DISPLAY_TEMP)
		display(tempActual);
	if (currentDMode == DMODE_SET_TEMP) {
		display(tempConsigne - 1);
	}
}

FrigoDisplay::FrigoDisplay(byte pcf8574Address) {
	setMode(DMODE_DISPLAY_TEMP);
	superFreezer = false;
}

byte FrigoDisplay::getMode() {
	return currentDMode;
}

void FrigoDisplay::setMode(byte DMode) {
	if (currentDMode != DMODE_SET_TEMP && DMode == DMODE_SET_TEMP)
		blink = true;
	if (DMode == DMODE_SET_TEMP)
		previousDModeSetTemp = millis();
	currentDMode = DMode;
}

void FrigoDisplay::setTempConsigne(float temp) {
	tempConsigne = temp;
}

void FrigoDisplay::setTempActual(float temp) {
	tempActual = temp;
}

void FrigoDisplay::display(float temp) {
	if (millis() - previous < REFRESH_DISPLAY_TIME
			&& FrigoDisplay::currentDMode == DMODE_DISPLAY_TEMP)
		return;

	if (temp < 0) {
		lightLed(LED_SuperFridge);
		return;
	}

	if (temp < 2) {
		lightLed(LED_T2);
		return;
	}
	if (temp < 4) {
		lightLed(LED_T4);
		return;
	}
	if (temp < 6) {
		lightLed(LED_T6);
		return;
	}

	lightLed(LED_T8);

	if (FrigoDisplay::currentDMode == DMODE_DISPLAY_TEMP)
		previous = millis();
}

void FrigoDisplay::lightLed(byte LED) {
	Wire.beginTransmission(PCF8574);
	Wire.write(0xff);
	byte error = Wire.endTransmission();
	Wire.beginTransmission(PCF8574);
	byte value = 0xff ^ (1 << LED);
	if (superFreezer)
		value ^= (1 << LED_SuperFreezer);
	Wire.write(value);
	error = Wire.endTransmission();
}

void FrigoDisplay::setSuperFreezer(boolean value) {
	superFreezer = value;
}
