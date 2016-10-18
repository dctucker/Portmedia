/*
 *  delay.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-03.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once

#include <cmath>
#include "main.h"
#include "param.h"
#define DELAY_LENGTH SAMPLE_RATE * 10

class Delay : public Set
{
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
	virtual void SetParam( int n, fl v );
	
	static fl square( fl f ){ return f * f; }
	static fl root( fl f ){ return sqrt(f); }
};
