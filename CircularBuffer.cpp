/*
 * CircularBuffer.cpp
 *
 *  Created on: Dec 5, 2018
 *      Author: yam
 */

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(int size_) {
	buffer = new float[size_];
	size = size_;
	currentIndex = 0;
}

void CircularBuffer::addValue(float value) {
	buffer[currentIndex] = value;
	if (nAdded < size)
		++nAdded;
	++currentIndex;
	if (currentIndex >= size)
		currentIndex = 0;
}

float CircularBuffer::getAverage() {
	if (nAdded == 0)
		return 0;
	if (nAdded < size)
		return buffer[0];

	float sum = 0;
	for (int i = 0; i < size; ++i) {
		sum += buffer[i];
	}

	return sum / size;
}
