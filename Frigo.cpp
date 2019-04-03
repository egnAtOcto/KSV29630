/*
 * Frigo.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: yam
 */

#include "Frigo.h"
#include "Config.h"
#include <Arduino.h>
#include <Wire.h>

Frigo::Frigo() {
}

void Frigo::setTemp(float newTemp) {
	tempConsigne = newTemp;
	if (tempConsigne == 0) {
		lastSuperFridgeTimestamp = millis();
		isSuperFridge = true;
	} else
		isSuperFridge = false;
}

void Frigo::setState(byte newState) {
	state = newState;
}

void Frigo::fridgeSwitchCallback() {
	if (display->getMode() != DMODE_SET_TEMP) {
		display->setTempConsigne(tempConsigne);
		display->setMode(DMODE_SET_TEMP);
		return;
	}
	display->setMode(DMODE_SET_TEMP);
	setTemp(tempConsigne - 2);

	if (tempConsigne < 0)
		setTemp(8);
	display->setTempConsigne(tempConsigne);
}

void Frigo::freezerSwitchCallback() {
	if (!isSuperFreezer)
		lastSuperFreezerTimestamp = millis();

	isSuperFreezer = !isSuperFreezer;
}

void Frigo::setSuperFridge(char* value) {
	if (strcmp(value, "ON") == 0)
		setTemp(0);
	else
		setTemp(Config::getInstance()->getInt("defaultTempConsigne"));
}

void Frigo::init() {
	mqtt = new MqttConnector(this);
	sensors = new Sensors(PCF8591);
	superFridgeNotif = new Notifier(mqtt, "fridge/superFridgeState");
	superFreezerNotif = new Notifier(mqtt, "fridge/superFreezerState");
	ambNotif = new Notifier(mqtt, "fridge/ambient");
	fridgeNotif = new Notifier(mqtt, "fridge/fridge");
	freezNotif = new Notifier(mqtt, "fridge/freezer");
	tempSettingNotif = new Notifier(mqtt, "fridge/tempSetting");
	stateNotif = new Notifier(mqtt, "fridge/state");
	comp = new Compressor(COMP_PIN);
	display = new FrigoDisplay(PCF8574);
	state = STANDBY;
	isSuperFridge = false;
	isSuperFreezer = false;
	lastSuperFridgeTimestamp = 0;
	lastSuperFreezerTimestamp = 0;

	mqtt->setSetTempHandler(&Frigo::setTemp);
	mqtt->setSetStateHandler(&Frigo::setState);
	mqtt->setSuperFridgeHandler(&Frigo::setSuperFridge);
	mqtt->setSuperFreezerHandler(&Frigo::freezerSwitchCallback);
	SUPERFRIDGE_MAX_TIME = Config::getInstance()->getInt("superFridgeMaxTime");
	SUPERFREEZER_MAX_TIME = Config::getInstance()->getInt("superFreezerMaxTime");
	SUPERFRIDGE_MIN_TEMP = Config::getInstance()->getInt("superFridgeMinTemp");
	SUPERFREEZER_MAX_TEMP = Config::getInstance()->getInt("superFreezerMaxTemp");
	tempConsigne=Config::getInstance()->getInt("defaultTempConsigne");
}

void Frigo::refresh() {
	ambientTemp = sensors->getAmbient();
	fridgeTemp = sensors->getFridge();
	freezerTemp = sensors->getFreezer();
	currentValue = sensors->getCurrent();

	// Too hot : start
	// Normal / SuperFridge / SuperFreezer
	if (state == STANDBY
			&& (fridgeTemp > tempConsigne
					|| (fridgeTemp > SUPERFRIDGE_MIN_TEMP) && (isSuperFridge || isSuperFreezer && freezerTemp > SUPERFREEZER_MAX_TEMP))) {
		if (comp->start())
			state = RUNNING;
	}

	// Normal operation (not super freezer or superfridge)
	// TempConsigne reached or stop requested : stop
	if (state == RUNNING && fridgeTemp <= tempConsigne && !isSuperFreezer && !isSuperFridge || state == OFF) {
		if (comp->stop()) {
			state = (state == OFF ? OFF : STANDBY);
		}
	}

	// SuperFridge or SuperFreezer
	// stop only if tempFridge < SUPERFRIDGE_MIN_TEMP
	if (state == RUNNING && (isSuperFridge || isSuperFreezer)) {
		if (fridgeTemp < SUPERFRIDGE_MIN_TEMP) {
			if (comp->stop()) {
				state = (state == OFF ? OFF : STANDBY);
			}
		}
	}

	// SuperFridge timeout
	if (isSuperFridge && millis() - lastSuperFridgeTimestamp > SUPERFRIDGE_MAX_TIME) {
		tempConsigne = Config::getInstance()->getInt("defaultTempConsigne");
		isSuperFridge = false;
	}

	// SuperFreezer timeout
	if (isSuperFreezer && millis() - lastSuperFreezerTimestamp > SUPERFREEZER_MAX_TIME) {
		isSuperFreezer = false;
	}

	if (isSuperFreezer)
		display->setSuperFreezer(true);
	else
		display->setSuperFreezer(false);

	stateNotif->newValue(state, false);
	ambNotif->newValue(ambientTemp, false);
	fridgeNotif->newValue(fridgeTemp, false);
	freezNotif->newValue(freezerTemp, false);
	tempSettingNotif->newValue(tempConsigne, false);
	superFridgeNotif->newValue((char*) (isSuperFridge ? "ON" : "OFF"), false);
	superFreezerNotif->newValue((char*) (isSuperFreezer ? "ON" : "OFF"), false);
	display->setTempActual(fridgeTemp);
	display->refresh();

	if (millis() >= maxRunTime)
		ESP.restart();
}

void Frigo::forceRefresh() {
	stateNotif->newValue(state, true);
	ambNotif->newValue(ambientTemp, true);
	fridgeNotif->newValue(fridgeTemp, true);
	freezNotif->newValue(freezerTemp, true);
	tempSettingNotif->newValue(tempConsigne, true);
	superFridgeNotif->newValue((char*) (isSuperFridge ? "ON" : "OFF"), true);
	superFreezerNotif->newValue((char*) (isSuperFreezer ? "ON" : "OFF"), true);
}

void Frigo::i2cScan() {
	byte error, address;

	for (address = 1; address < 127; address++) {
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0) {
			mqtt->publish("fridge/i2cdevice", address);
		} else if (error == 4) {
			mqtt->publish("fridge/i2cerror", address);
		}
	}
}
