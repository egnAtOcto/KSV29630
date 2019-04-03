#include "MqttConnector.h"
#include "Sensors.h"
#include "Compressor.h"
#include "FrigoDisplay.h"
#include "Notifier.h"
#include "pins.h"

/*
 * Frigo.h
 *
 *  Created on: Nov 23, 2018
 *      Author: yam
 */

/*
 * Pins connected as follow
 * SDA		ESP8266 D2/GPIO4
 * SCL		ESP8266 D1/GPIO5
 * Ambient	PCF8591	AIN0
 * Fridge	PCF8591	AIN1
 * Freezer	PCF8591	AIN2
 * Current	PCF8591	AIN3
 * FridgeSwitch	ESP	D5
 * FreezerSwitch	ESP	D7
 */

#ifndef FRIGO_H_
#define FRIGO_H_

#define OFF 0
#define STANDBY 1
#define RUNNING 2
#define FAILURE 3

class Frigo {
private:
	unsigned long lastSuperFridgeTimestamp;
	unsigned long lastSuperFreezerTimestamp;
	unsigned long maxRunTime = 0x9a7ec800; //30 days

	boolean isSuperFridge;
	boolean isSuperFreezer;
	byte state;
	MqttConnector * mqtt;
	Sensors * sensors;
	Compressor * comp;
	Notifier * tempSettingNotif;
	Notifier * ambNotif;
	Notifier * fridgeNotif;
	Notifier * freezNotif;
	Notifier * stateNotif;
	Notifier * superFridgeNotif;
	Notifier * superFreezerNotif;
	FrigoDisplay * display;
	float tempConsigne;
	float ambientTemp;
	float fridgeTemp;
	float freezerTemp;
	float currentValue;

	// Config below
	int SUPERFRIDGE_MIN_TEMP;
	int SUPERFREEZER_MAX_TEMP;
	int SUPERFRIDGE_MAX_TIME;
	int SUPERFREEZER_MAX_TIME;

public:
	Frigo();
	void init();
	void refresh();
	void forceRefresh();
	void setTemp(float newTemp);
	void setState(byte newState);
	void fridgeSwitchCallback();
	void setSuperFridge(char*value);
	void freezerSwitchCallback();
	void i2cScan();
};

#endif /* FRIGO_H_ */
