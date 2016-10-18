/*
 *  compress.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-04-28.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once 

#include "main.h"

#define NOP -1.

typedef float   stereodata[2];
class Compress {

public:

	float*  wav_in;  // signal
	double  thresh;  // threshold (percents)
	double  slope;   // slope angle (percents)
	double  tla;     // lookahead  (ms)
	double  twnd;    // window time (ms)
	double  tatt;    // attack time  (ms)
	double  trel;    // release time (ms)

	double att, rel, env;
	int lhsmp, nrms;
	double rms, theta, gain;


	Compress(
		double  thresh_ = 0.05,   // threshold (percents)
		double  slope_  = 0.1,   // slope angle (percents)
		double  tla_    = 2.0,   // lookahead  (ms)
		double  twnd_   = 4.0,   // window time (ms)
		double  tatt_   = 0.8,   // attack time  (ms)
		double  trel_   = 300.   // release time (ms)
	);
	~Compress();
	
	void setup();
	void set(
		double t = NOP, double s = NOP,
		double la = NOP, double w = NOP,
		double a = NOP, double r = NOP
	);
	
	void operator()(long unsigned int , float **);

};
