/*
 *  bus.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-06.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "bus.h"

Bus::Bus() :
	masterVol( 1.0 ), dist( 1.0 ), gens(0), gain( 1.0 ),
	masterVolEnv( 1.0 ),
	//delay( new Delay() ),
	filtLP(new BiQuad(LPF)), filtHP(new BiQuad(HPF)),
	//compress(new Compress()),
	numSends( 1 ),
	pos(0), width(349*2),

	env(0.0),
	attack_coef(  exp(log(0.01)/( 0.004 * SAMPLE_RATE ) ) ),
	release_coef( exp(log(0.01)/( 0.400 * SAMPLE_RATE ) ) ),
	ratio(4.0f),
	thresh( powf( 10.f, -12.f / 20.f ) )

{

	for(int i=0; i < SAMPLE_RATE; i++){
		minmaxv[i] = 0.0;
	}

	for(int i=0; i < 128; i++){
		gen[i] = NULL;
		atten[i] = 1.0;
		clip[i] = 1.0;
		for(int j=0; j < 8; j++){
			send[i][j] = 0.0;
			sendacc[j][0] = new float[SAMPLE_RATE];
			sendacc[j][1] = new float[SAMPLE_RATE];
		}
	}
	
	buf[0] = new float[SAMPLE_RATE];
	buf[1] = new float[SAMPLE_RATE];
	
	acc[0] = new float[SAMPLE_RATE];
	acc[1] = new float[SAMPLE_RATE];
	
	setFilter(0.0f);
}

Bus::~Bus()
{
	//delete delay;
	delete filtLP; delete filtHP;
	//delete compress;
	delete buf[0]; delete buf[1];
	delete acc[0]; delete acc[1];
}

void Bus::setDelay(float d, float v)
{
	//delay->set(d,v);
}

void Bus::setSend( int i, int o, float v )
{
	send[i][o] = v;
}

void Bus::add(Generator *g)
{
	gen[gens++] = g;
}
void Bus::setVol(float f)
{
	masterVol = f;
}
void Bus::setDist(float f)
{
	dist = f;
}
void Bus::setClip(int i, float f)
{
	clip[i] = f;
}

void Bus::setFilter(float v)
{
	float lpf, hpf, lpr, hpr;
	
	v = 1.0 - v;
	lpf = 150.0f * powf( 2, 6.5f * 1.0f * fabs(1.0f - v * 2.0f) );

	if( v < 0.5 ) // lowpass mode
	{	
		hpf = 32.5f;
		lpr = 1.0f;
	}
	else // highpass mode
	{
		float v1 = v - 0.5f;
		//v1 += 0.1; v1 = min( v, 0.5f );
		hpf =  32.5f * powf( 2, 6.0f * 2.0f * max( v1 - 0.1, 0.0 ) );
		
		
		//if( (prev_v - v) < 0 ) // descending
		{
			lpr = 1.0f;
			hpr = 1.0f - 0.95f * 2.0f * v1;
		}
		/*else // ascending
		{
			lpr = 1.0f - 0.95f * 2.0f * v1;
			hpr = 1.0f;
		}*/
	}
	
	
	
	//hpr = 1.0 - 0.95 * 2.0 * fmax(0.0f, v - 0.5f);
	hpr = 1.0;
	
	filtLP->set( lpf , lpr ); //+ 12.0f * powf( 2.0f, 4.0f * fmax(0.0, v - 0.75) ) );
	filtHP->set( hpf , hpr ); //+ 18.0f * powf( 2.0f, 4.0f * fmax(0.0, v - 0.75) ) );
	//gain = 1.0 + 4.0 * fmax( 0.0f, v - 0.75f );
	prev_v = v;

}


fl Bus::queryFilter(fl f)
{
	return filtLP->query(f) + filtHP->query(f) ;
}

int Bus::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status)
{
	assert(outBuf != NULL);
	float **out = OUT_CAST(outBuf);

	//float 
	//if( acc[0] == NULL )
	//	acc[0] = new float[framesPerBuf];
	
	for(int i=0; i < framesPerBuf; i++)
	{
		acc[0][i] = acc[1][i] = 0.0f;
		/*
		for(int j=0; j < numSends; j++){
			sendacc[j][0][i] = 0.0f;
			sendacc[j][1][i] = 0.0f;
		}
		*/
	}
	
	asm("# Init done");
	
	float o, o0, o1, maxo = 0.0f, mino = 0.0f;

	//#pragma omp parallel for
	for(int k=0; k < gens; k++)
	{
		//float maxv = 0;
		if( gen[k]->active )
		{
			gen[k]->generate(NULL, buf, framesPerBuf, timeInfo, status);

			for(int i=0; i < framesPerBuf; i++)
			{
				acc[0][i] += buf[0][i]; // atten[k];
				acc[1][i] += buf[1][i]; // atten[k];

				// SENDS
				//for(int j=0; j < numSends; j++){
				//	sendacc[j][0][i] += send[k][j] * buf[0][i];
				//}

				//acc[1][i] += buf[1][i]; // atten[k];
			}
		}

	}
	
	/*// SENDS
	for(int i=0; i < framesPerBuf; i++){ // apply sends
		for( int j=0; j < numSends; j++){
			acc[0][i] += delay->process(sendacc[j][0][i]);
		}
	}
	// */
	
	asm("# Generators done");
	
	for(int i=0; i < framesPerBuf; i++)
	{
		o0 = acc[0][i];
		o1 = acc[1][i];
		
		//o = delay->process(o);
		//delay->advance();
		
		// apply master lowpass / highpass chain here from mod wheel
		
		o0 = filtLP->filter(o0);
		o0 = filtHP->filter(o0) * gain;

		o1 = filtLP->filter_r(o1);
		o1 = filtHP->filter_r(o1) * gain;
		
		//o0 *= masterVol;
		//o1 *= masterVol;
		
		acc[0][i] = o0;
		acc[1][i] = o1;
	
		//compute min/max
		o = o0 + o1;
		maxo = fmax(maxo, o);
		mino = fmin(mino, o);
		
	}

	inp = fmin( 1.0, fmax(maxo,-mino) );


	#define FOLLOW( INV, OUTV ) \
		if( (INV) > (OUTV) ) \
			OUTV = attack_coef  * ((OUTV) - (INV)) + (INV); \
		else \
			OUTV = release_coef * ((OUTV) - (INV)) + (INV);
	
	maxo = acc[0][0]; mino = acc[0][0];	
	for(int i=0; i < framesPerBuf; i++)
	{
		// calculate compression envelope
		FOLLOW( inp, env )

		ds = thresh / env;
		cmp = fminf( 1.f, ds / powf( ds, 1.f / ratio ) );
	
		///*
		// apply gain reduction
		acc[0][i] *= cmp;
		acc[1][i] *= cmp;
		//*/

		FOLLOW( masterVol, masterVolEnv )
		
		// apply master volume fader
		acc[0][i] *= masterVolEnv;
		acc[1][i] *= masterVolEnv;

		//std::cout << env << "\n";

		//compute min/max
		o0 = acc[0][i];
		o1 = acc[1][i];
		o = o0 + o1;
		maxo = fmax(maxo, o);
		mino = fmin(mino, o);
		
		
		// set both of these to o for mono mix
		// or to o0, o1 for stereo
		out[0][i] = o0;
		out[1][i] = o1;

		// oscilloscope
		if( i % 10 == 0 )
		{
			//sc->append(maxo, mino);

			minmaxv[pos++] = mino; //-sqrtf(-mino);
			minmaxv[pos++] = maxo; // sqrtf( maxo);

			if(pos > width) pos = 0;

			maxo = 0.0;
			mino = 0.0;
		}
	}
	
	//sc->Refresh();
	//evt = new wxPaintEvent();
	//::wxPostEvent( sc, *evt );
	
	//level = maxo;
	//delete [] buf;
	count += framesPerBuf;
	return paContinue;
}
