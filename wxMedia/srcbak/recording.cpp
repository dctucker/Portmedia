/*
 *  recording.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2009-11-15.
 *  Copyright 2009 UNCW. All rights reserved.
 *
 */

#include "recording.h"


int Recorder::consume CONSUME(inBuf, outBuf, framesPerBuf, timeInfo, status) {
	assert(inBuf != NULL);
	float **in = IN_CAST(inBuf);
	//float **in = static_cast<float **> (const_cast<void *> (inBuf) );
		// that's right, where's your constness now?!

	for(int i=0; i < framesPerBuf; i++){
		buf[0][cursor] = in[0][i];
		buf[1][cursor] = in[1][i];
		cursor += 1;
		cursor %= num_samples;
	}

	return paContinue;
}

Recorder::Recorder(){
	cursor = 0;
	num_samples = SAMPLE_RATE * 30;
	buf = new float *[2];
	buf[0] = new float[num_samples];
	buf[1] = new float[num_samples];
}

float **Recorder::GetSamples(){
	return buf;
}

void Recorder::Rewind(){
	cursor = 0;
}
