/*
 *  recording.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2009-11-15.
 *  Copyright 2009 UNCW. All rights reserved.
 *
 */
#include "portaudiocpp/PortAudioCpp.hxx"
#include "main.h"

#define IN_CAST( x ) static_cast<float **> (const_cast<void *> ((x)) )

#define CONSUME(X,Y,Z,W,S) ( const void * (X), void * (Y), unsigned long (Z), const PaStreamCallbackTimeInfo *(W), PaStreamCallbackFlags (S) )


class Consumer {
public:
	virtual int consume CONSUME(inBuf, outBuf, framesPerBuf, timeInfo, status) = 0;
};

class Recorder : public Consumer {
public:
	float **buf;
	int cursor, num_samples;
	int consume CONSUME(inBuf, outBuf, framesPerBuf, timeInfo, status);	
	Recorder();
	float **GetSamples();
	void Rewind();
};
