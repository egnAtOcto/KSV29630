/*
 * TempConverter.h
 *
 *  Created on: Nov 19, 2018
 *      Author: yam
 */

#include <Arduino.h>

#ifndef TEMPCONVERTER_H_
#define TEMPCONVERTER_H_

class TempConverter {
private:
	float SERIESRESISTOR;
	float NOMINAL_RESISTANCE;
	float NOMINAL_TEMPERATURE;
	float BCOEFFICIENT;

public:
	TempConverter();
	float getTemperatureFrom8bits(byte adcValue);
};

#endif /* TEMPCONVERTER_H_ */
