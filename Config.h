/*
 * Config.h
 *
 *  Created on: Dec 1, 2018
 *      Author: yam
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include <string>
#include <hash_map>
#define NUM_CHAR_PARAMS 31

class Config {
private:
	static Config * instance;
	const char * strParams[NUM_CHAR_PARAMS] = { "ssid", "wifiPassword", "mqttServer", "mqttUser", "mqttPass", "superFridgeState", "superFreezerState",
			"ambientTemp", "fridgeTemp", "freezerTemp", "tempSetting", "fridgeState", "setSuperFridge", "setSuperFreezer", "setTemp", "reset",
			"onoff", "mqttPort", "seriesResistor", "nominalResistance", "nominalTemperature", "bCoeff", "compMinRunTime", "compTimeBeforeRestart",
			"superFridgeMaxTime", "superFridgeMinTemp", "superFreezerMaxTemp", "superFreezerMaxTime", "sampleTime", "numSamples", "mqttClientId" };

	// Values below are default values if spiffs unavailable
	String strValues[NUM_CHAR_PARAMS];
	String defaultValues[NUM_CHAR_PARAMS] = { "ssid", "password", "192.168.0.1", "", "", "fridge/superFridgeState", "fridge/superFreezerState",
			"fridge/ambient", "fridge/fridge", "fridge/freezer", "fridge/tempSetting", "fridge/state", "fridge/setSuperFridge",
			"fridge/setSuperFreezer", "fridge/setTemp", "fridge/reset", "fridge/onoff", "1883", "12700", "5000", "25", "3820", "300000", "300000",
			"10800000", "-1", "-20", "10800000", "10000", "3", "Bosch KSV29630" };
	Config();

public:
	static Config * getInstance();
	String get(const char * paramName);
	int getInt(const char * paramName);
	float getFloat(const char * paramName);
	void clear();
};

#endif /* CONFIG_H_ */
