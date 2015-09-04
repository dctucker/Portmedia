/*
 *  delay.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-03.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#ifndef DELAY_H
#define DELAY_H

#include "main.h"
#define DELAY_LENGTH SAMPLE_RATE * 10

class Delay {
public:
	int pos, dpos;
	int delay;
	float feedback;
	float buffer[DELAY_LENGTH];

	Delay();
	float process(float s);
	void process(float **s, int N);
	void advance(int N=1);
	void set(float d=-1,float f=-1);
};

#endif
