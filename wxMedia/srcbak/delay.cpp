/*
 *  delay.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-03.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "delay.h"

Delay::Delay() : pos(0), delay(SAMPLE_RATE/3), feedback(0.5)
{
	for(int i = 0; i < DELAY_LENGTH; ++i){
		buffer[i] = 0.0;
	}
}

float Delay::process(float s)
{
	if(pos >= DELAY_LENGTH) pos = 0;
	dpos = pos - delay;
	if(dpos < 0) dpos += DELAY_LENGTH;
	float r = s + (buffer[dpos] * feedback);
	buffer[pos] = r;
	++pos;
	return r;
}

void Delay::process(float **s, int N){
	int n = pos;
	for(int i=0; i < N; ++i){
		s[0][i] += process(s[0][i]);
	}
	pos = n;
}

void Delay::advance(int N){
	pos += N;
}

void Delay::set(float d, float f){
	if( d != -1 ) delay = SAMPLE_RATE * d;
	if( f != -1 ) feedback = f;
}
