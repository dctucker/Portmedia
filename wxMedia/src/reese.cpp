/*
 *  reese.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-06.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "reese.h"

Reese::Reese()
{
	setDetune(0.2);
	wt = new SawTbl();
	
	AddParam( &detval, "Detu" );
}

void Reese::SetParam( int n, fl v )
{
	switch( n )
	{
		case 0x75746544:  // Detu
			setDetune( v );
			break;
		default:
			Oscillator::SetParam( n, v );
	}
}

void Reese::setDetune(float f)
{
	detval = f;
	if( detval > 0.0 )
	{
		detune = pow(2, detval * 30./1200.);
		atten = 0.5 * (1 + 0.25 * detval);
	}
	else 
	{
		detune = 1.0;
		atten = 0.5;
		detval = 0.0;
	}
	retemper();
}

void Reese::retemper(){
	for(int n=0; n < 128; n++){
		//notes[n].freq = calcFreq(n);
		notes[n].phase  *= calcFreq(n) * detune;
		notes[n].phase2 *= calcFreq(n) / detune;
	}
}

int Reese::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status)
{
	assert(outBuf != NULL);
	float **out = static_cast<float **>(outBuf);

	float sL, sR;
	//int p1, f1, p2, f2;
	//int frameClk;
	float o;

	//float filt[2][SAMPLE_RATE];// = new float[2][SAMPLE_RATE];

	for(int i=0; i < framesPerBuf; i++){
		++clk;
		sL = 0.0f;// sR = 0.0;
		//frameClk = clk | ADSR_MASK;
		
		//if(!modvec){
		
		for(int n = lowNote; n <= highNote; n++)
		{
			noteData &note = notes[n];

			if(note.num != 0 )
				doADSR(n);
		
			if(note.num != 0)
			{
				//note.time++;
				/*
				p1 = note.phase; // >> SHIFT; //& MASK;
				p2 = note.phase2;// >> SHIFT; //& MASK;
				f1 = note.freq * detune;
				f2 = note.freq / detune;
				*/
				sL += 0.5f * wt->table[note.phase++] * note.amp * 
					 (0.5 * note.vel + 0.5f) * atten;
				sL += 0.5f * wt->table[note.phase2++] * note.amp * 
					 (0.5f * note.vel + 0.5f) * atten;

				//note.phase  += f1;
				//note.phase2 += f2;
				
				//if(notes[n].phase >= tableSize)
				//	notes[n].phase -= tableSize;
					
				//if(notes[n].phase2 >= tableSize)
				//	notes[n].phase2 -= tableSize;
			}
		
		}
		
		o = applyEffects(sL);

		
		sL = o * pan->ampL;
		sR = o * pan->ampR;
		out[0][i] = sL;
		out[1][i] = sR;
		
	}
	return paContinue;	
}

///////////////////////////////////////////////////////////////////////////////


SubSineTbl::SubSineTbl()
{
	double f = 2. * M_PI / (double)tableSize;
	for( int t = 0; t < tableSize; ++t )
	{
		table[t] =  0.5f * sinf(t * f);       // fundamental
		table[t] += 0.2f * sinf(t * 2.0 * f); //    +8va
		table[t] += 0.3f * sinf(t * 0.5 * f); //    -8va
	}
}

SubSineOsc::SubSineOsc() : 	curNote(127), prevNote(127)
{
	wf = new SubSineTbl();
}

void SubSineOsc::mark(double t){ time = t; }

void SubSineOsc::duck()
{
	
}

void SubSineOsc::noteOn(int k, int v)
{

	//std::cout << k << " on  " << time << "\n";
/* new idea:
	bass note should be stored during single note events.
	when a chord is played (notes in sequence with minimal time distance),
		recall bass note and play it instead
*/
	//float thresh = 0.01;

	int s = sus;
	sus = 0;
	if( v < 64 ) v = 64;
	Oscillator::noteOn(k, v);

	notes[k].down = true;

	//if( k < curNote )
	
	//if( time > thresh || time == 0 ){
		prevNote = k;
	//	std::cout << prevNote << " adj " << time << "\n";
		curNote = k;
	//} else {
	//	std::cout << prevNote << " adj " << time << "\n";
	//	curNote = prevNote;
	//	`q}
	
	noteData &note = notes[curNote];

	note.down = true;
	
	/*
	// make sure the frequency is in range, otherwise double or halve.
	const static int testVal1 =  35 * (((float)resolution) / (float)SAMPLE_RATE);
	const static int testVal2 = 135 * (((float)resolution) / (float)SAMPLE_RATE);
	int freq;
	while( freq < testVal1 )
		freq *= 2;
	while( freq > testVal2 )
		freq /= 2;
	note.phase *= freq;
	*/
	
	int hi;
	for( hi = 0; hi < 128; ++hi )
	{
		if( notes[hi].ampEnv.stage != '_' and hi != curNote )
		{
			bool d = notes[hi].down;
			Oscillator::noteOff(hi);
			notes[hi].down = d;
		}
	}
	sus = s;
	
}

void SubSineOsc::noteOff(int k)
{
	//std::cout << k << " off " << time << "\n";
	Oscillator::noteOff(k);
	
	notes[k].down = false;
	//notes[k].stage = 'R';
	//notes[k].time = 0;
	
	//if( time > 0.04 ){
	//	curNote = k;
	//}
	
	int i;
	for( i = 0; i < 127; ++i )
	{
		if( notes[i].down )
		{
			curNote = i;
			noteOn(curNote, notes[curNote].vel);
			//notes[i].stage = 'A';
			//notes[i].time = 0;
			return;
		}
		curNote = 127;
	}
}

