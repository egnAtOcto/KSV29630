/*
 * FrigoDisplay.h
 *
 *  Created on: Nov 20, 2018
 *      Author: yam
 */

#include <Arduino.h>
#include <Ticker.h>
#include "pins.h"

#ifndef FRIGODISPLAY_H_
#define FRIGODISPLAY_H_

#define DMODE_DISPLAY_TEMP 0
#define DMODE_SET_TEMP 1
#define LED_T2 3
#define LED_T4 2
#define LED_T6 1
#define LED_T8 0
#define LED_SuperFridge 4
#define LED_SuperFreezer 5

class FrigoDisplay {
private:
	unsigned long previous;
	unsigned long previousDModeSetTemp;
	byte currentDMode;
	boolean superFreezer;

	float tempActual;
	float tempConsigne;
	byte pcf8574Address;
	void lightLed(byte LED);
	void display(float temp);

public:
	FrigoDisplay(byte pcf8574Address);
	void setTempConsigne(float newTemp);
	void setTempActual(float actualTemp);
	void setMode(byte DMODE);
	void setSuperFreezer(boolean value);
	byte getMode();
	void refresh();
};

#endif
