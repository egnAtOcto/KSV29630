/*
 * CircularBuffer.h
 *
 *  Created on: Dec 5, 2018
 *      Author: yam
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

class CircularBuffer {
private:
	int nAdded=0;
	float * buffer;
	int size;
	int currentIndex;
public:
	CircularBuffer(const int size);
	float getAverage();
	void addValue(float value);
};

#endif /* CIRCULARBUFFER_H_ */
