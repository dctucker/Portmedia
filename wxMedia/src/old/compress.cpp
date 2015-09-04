/*
 *  compress.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-04-28.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "compress.h"
#include <cmath>


Compress::Compress(
	double thresh_, double slope_,
	double tla_, double twnd_,
	double tatt_, double trel_
) :
	thresh(thresh_),
	slope(slope_),
	tla(tla_),
	twnd(twnd_),
	tatt(tatt_),
	trel(trel_)
{
	setup();
}

Compress::~Compress()
{
}

void Compress::set(double t, double s, double la, double w, double a, double r)
{
	if( t  != NOP ) thresh = t;
	if( s  != NOP ) slope = s;
	if( la != NOP ) tla = la;
	if( w  != NOP ) twnd = w;
	if( a  != NOP ) tatt = a;
	if( r  != NOP ) trel = r;
	
	setup();

}


void Compress::setup()
{
	thresh *= 0.01;     // threshold to unity (0...1)
	slope  *= 0.01;     // slope to unity
	tla    *= 1e-3;     // lookahead time to seconds
	twnd   *= 1e-3;     // window time to seconds
	tatt   *= 1e-3;     // attack time to seconds
	trel   *= 1e-3;     // release time to seconds

	// attack and release "per sample decay"
	att = (tatt == 0.0) ? (0.0) : exp (-1.0 / (SAMPLE_RATE * tatt));
	rel = (trel == 0.0) ? (0.0) : exp (-1.0 / (SAMPLE_RATE * trel));

	lhsmp = (int) (SAMPLE_RATE * tla); // sample offset to lookahead wnd start
	nrms = (int) (SAMPLE_RATE * twnd); // samples count in lookahead window

	env = 0.0;   // envelope
}

void Compress::operator() (long unsigned int n, float **wav)
{
	// for each sample...
	for (int i = 0; i < n; ++i)
	{
		// now compute RMS
		double  summ = 0;

		// for each sample in window
		for (int j = 0; j < nrms; ++j)
		{
			int     lki = i + j + lhsmp;
			double  smp;

			// if we in bounds of signal?
			// if so, convert to mono
			if (lki < n)
				smp = 0.5 * wav[0][lki] + 0.5 * wav[1][lki];
			else
				smp = 0.0;      // if we out of bounds we just get zero in smp

			summ += smp * smp;  // square em..
		}

		rms = sqrt (summ / nrms);   // root-mean-square

		// dynamic selection: attack or release?
		theta = rms > env ? att : rel;

		// smoothing with capacitor, envelope extraction...
		// here be aware of pIV denormal numbers glitch
		env = (1.0 - theta) * rms + theta * env;

		// the very easy hard knee 1:N compressor
		gain = 1.0;
		if (env > thresh)
			gain -= (env - thresh) * slope;

		wav[0][i] *= gain;
		wav[1][i] *= gain;
	}
}
