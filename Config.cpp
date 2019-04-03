/*
 * Config.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: yam
 */

#include <FS.h>
#include "Config.h"
#include <string.h>

Config * Config::instance;
Config::Config() {
	bool result = SPIFFS.begin();
	clear();
}

Config * Config::getInstance() {
	if (Config::instance == nullptr)
		Config: instance = new Config();

	return Config::instance;
}

void Config::clear() {
	for (int i = 0; i < NUM_CHAR_PARAMS; ++i) {
		strValues[i] = "";
	}
}

String Config::get(const char * paramName) {
	String paramFileName = String("/param/") + String(paramName);
	for (int i = 0; i < NUM_CHAR_PARAMS; ++i) {
		if (strncmp(strParams[i], paramName, strlen(strParams[i])) == 0)
			if (strValues[i].length() == 0) {
				File file = SPIFFS.open(paramFileName, "r");
				if (!file) {
					Serial.println("File unavailable ; getting default");
					strValues[i] = defaultValues[i];
				} else {
					String value = file.readStringUntil('\n');
					strValues[i] = value;
					file.close();
					return strValues[i];
				}
			} else
				return strValues[i];
	}
	Serial.println("No value found for " + paramFileName + "==");
	return String("0123456789");
}

int Config::getInt(const char * paramName) {
	String value = get(paramName);
	return value.toInt();
}

float Config::getFloat(const char * paramName) {
	String value = get(paramName);
	return value.toFloat();
}
