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
	masterVol( 1.0 ), dist( 1.0 ), gens(0),
	delay( new Delay() ),
	numSends( 1 )
{
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
	
}

void Bus::setDelay(float d, float v)
{
	delay->set(d,v);
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

int Bus::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status) {
	assert(outBuf != NULL);
	float **out = OUT_CAST(outBuf);

	//float 
	//if( acc[0] == NULL )
	//	acc[0] = new float[framesPerBuf];
	
	for(int i=0; i < framesPerBuf; i++){
		acc[0][i] = acc[1][i] = 0.0f;
		/*
		for(int j=0; j < numSends; j++){
			sendacc[j][0][i] = 0.0f;
			sendacc[j][1][i] = 0.0f;
		}
		*/
	}
	
	asm("# Init done");
	
	float o, maxo = 0.0f, mino = 0.0f;

	//#pragma omp parallel for
	for(int k=0; k < gens; k++){
		//float maxv = 0;
		if( gen[k]->active ){
			gen[k]->generate(NULL, buf, framesPerBuf, timeInfo, status);
			/*
			// Clipping code
			for(int i = 0; i < framesPerBuf; ++i){
				o = buf[0][i];
				ao = fabs(o);
				if( ao > clip[k] ){
					if( o == ao )
						buf[0][i] = clip[k];
					else
						buf[0][i] = -clip[k];
				}
			}
			*/
			
			/* // volume indicator
			for(int i=0; i < framesPerBuf; ++i){
				maxv = max( fabs( buf[0][i] / atten[k] ) , maxv );
			}
			if(maxv > atten[k])
				atten[k] = maxv;
			*/
			
			for(int i=0; i < framesPerBuf; i++){

				acc[0][i] += buf[0][i]; // atten[k];
				
				
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
	//*/
	
	asm("# Generators done");
	
	for(int i=0; i < framesPerBuf; i++){
		o = acc[0][i];
		
		//o = delay->process(o);
		//delay->advance();
		
		o *= masterVol;

		//if(o > maxo) maxo = o;
		//if(o < mino) mino = o;
		maxo = fmax(maxo, o);
		mino = fmin(mino, o);

		out[0][i] = o;
		out[1][i] = o; //acc[1][i] * masterVol;

		if( i % 10 == 0 ){
			sc->append(maxo, mino);
			maxo = 0.0;
			mino = 0.0;
		}
	}
	
	//sc->Refresh();
	//evt = new wxPaintEvent();
	//::wxPostEvent( sc, *evt );
	
	level = maxo;
	//delete [] buf;
	return paContinue;
}
