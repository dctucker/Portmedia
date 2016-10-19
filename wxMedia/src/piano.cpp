/*
 *  piano.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-13.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "piano.h"

using namespace std;

UprightLoader::UprightLoader(float ***s, bool *b) :
	wxThread(),
	samples(s),
	loaded(b)
{	
}

void UprightLoader::loadSample(int i){

	loaded[i] = true;
	/*
	static double const retune[88] = // starting with low A-1 and ending at high C+8
	{
		30., -20.,  38., -11.,  15.,   9.,  33.,   6.,  33.,  24.,  35.,  43.,
		13.,  22.,  42.,  42.,  22.,  17.,  19.,  24.,  40.,   7.,  33.,  34.,
		10.,  19.,  40.,  28.,  15.,  18.,  20.,  19.,  26.,  17.,   5.,  12.,
		 0.,   0.,   9.,   0.,   0.,  10.,   0.,   9.,   0.,  17.,   0.,   7.,
		10.,   3.,  12.,  10.,   0.,   8.,   2.,   9.,   0.,   0., -10.,   0.,
		19.,   0.,   0.,   0.,  25.,  12.,  16.,   9.,  21.,   0.,  40.,  24.,
		 0.,  41.,  30.,  30.,  35.,  26.,  20.,   9.,  32.,  15.,  33.,  30.,
		 0.,   0.,   0., -120. // last C note is rooted at C# and tuned down 20 cents	
	};
	*/
	/*
	static double const atten[88] =
	{
		-6.0,  0.0,  0.0, -0.2, 0.0, -0.8, 0.0, -2.0,  0.0,  0.0,  0.0,  0.0,
		 0.0,  0.0, -1.2,  0.0, 0.0, -0.8, 0.0,  0.0,  0.0,  0.0, -1.0,  0.0,
		-4.0, -6.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0, -1.0, -1.2, -3.1,  0.0,
		-2.6,  0.0,  0.0, -2.4, 0.0, -4.2, 0.0,  0.0, -4.0, -1.0,  0.0,  0.0,
		 0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0, -1.8,	 0.0,  0.0, -6.0,
		 0.0, -4.8,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
		 0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
		 0.0,  0.0,  0.0,  0.0
	};
	*/
	const char *note_name[12] = {
		"A ", "A#",
		"B ",
		"C ", "C#",
		"D ", "D#",
		"E ",
		"F ", "F#",
		"G ", "G#",
	};
	
	int oct = (21 + i) / 12 - 1;

	wxString filename = Oscillator::GetSamplePath();
	//filename << _T("_") << (i + 21) << _T(".aif.wav");
	filename << wxString(note_name[i % 12], wxConvUTF8) << oct << _T(".wav");
	std::cout << "Loading " << filename.mb_str() << "\n";

	SndfileHandle *sf = new SndfileHandle(filename.mb_str(), SFM_READ);

	if( not sf->error() )
	{
		sf->read(samples[i + 21][0], SAMPLE_LENGTH );

		/* // no more resampling now that piano is tuned
		//sf->read(samples[i % 21][0], SAMPLE_LENGTH );

		// it's in memory, now to resample
		double ratio = pow( 2, ( 30. + retune[i] ) / 1200. );
		SRC_DATA sd;
		sd.data_in       = samples[ i % 21 ][  0  ];
		sd.data_out      = samples[ i + 21 ][  0  ];
		sd.input_frames  = SAMPLE_LENGTH;
		sd.output_frames = SAMPLE_LENGTH;
		sd.src_ratio = 1./ ratio;
		src_simple(&sd, SRC_SINC_MEDIUM_QUALITY, 1);
		*/
		/*
		// finally, apply attenuation
		float att = powf( 10, 0.05f * ( 3.0 + atten[i] ) );
		for(int j=0; j < SAMPLE_LENGTH; ++j){
			samples[ i + 21][0][j] *= att;
		}
		*/

	} else {
		cout << filename << ": error loading sample" << endl;
		exit(3);
	}

	delete sf;
}

void *UprightLoader::Entry()
{
	for(int i = 0; i < 44; ++i){
		if( not loaded[44 - i] )
			loadSample(44 - i);
		if( not loaded[44 + i] )
			loadSample(44 + i);
	}
	if( not loaded[0] )
		loadSample(0);
	return NULL;
}

void UprightLoader::OnExit()
{
}


///////////////////////////////////////////////////////////////////////////////

Upright::Upright() :
	samples( new float ** [128] ),
	keyfilt( new FilterIIR77 ),
	mix( 0.0 ),
	mix0( 0.0 ), mix1( 1.0 ), mix2( 0.9 )
{

	//wt = new WaveTable();
	keyfilt->setRange(200., 10000.);

	pitchbend = 1024;

	//tableSize = 96000;
	for(int i = 0; i < 128; ++i)
	{
		loaded[i] = false;
		samples[i] = new float*[2];
		samples[i][0] = new float[SAMPLE_LENGTH];
		//notes[i].freq = 1;
		notes[i].phase >>= 10;
		notes[i].phase2 >>= 10;
		notes[i].phase3 >>= 10;
		//trigger(notes[i]);
	}
	
	Oscillator::AddParam(&mix, "Mix");
}

Upright::~Upright()
{
	for(int i = 0; i < 88; ++i)
	{
		delete samples[i][0];
		delete samples[i];
	}
	delete[] samples;
	delete keyfilt;
	//delete filt;
}

void Upright::SetParam( int f, fl v )
{
	switch( f )
	{
		case 0x78694D:  // Mix
			setMix( v );
			break;
		default:
			Oscillator::SetParam( f, v );
	} 
}

void Upright::trigger(noteData &note)
{
	int f = pitchbend * (*temper)(note.num) / tet(note.num);
	
	note.phase  *= f;
	note.phase2 *= f / 2;
	note.phase3 *= f * 2;
}


void Upright::loadSamples()
{
	load = new UprightLoader(samples, loaded);
	load->Create();
	load->Run();
}


void Upright::noteOn(int n, int v)
{
	if( not loaded[n - 21] and not loaded[n - 21 - 12] and not loaded[n - 21 + 12])
	{
		//this->loadSample( n - 21 );
	}
	else
	{
		Oscillator::noteOn(n,v);
		notes[n].vel = (notes[n].vel > 0.1) ? notes[n].vel : 0.1;
		notes[n].phase = 0;
		notes[n].phase2= 0;
		notes[n].phase3= 0;
		//notes[n].freq = 1.0;
		//notes[n].phase >>= 10;
		trigger(notes[n]);
		keyfilt->set(n,v);
	}
}

void Upright::bend(float b){
	pitchbend = 1024 * powf(2, bendInt * b / 12. );
	Upright::retemper();
}

void Upright::retemper() {
	for(int n=0; n < 128; n++){
		if(notes[n].num != 0){
			//notes[n].freq = pitchbend; //* ((*temper)(n) / tet(n));
			trigger(notes[n]);
		}
	}
}


void Upright::setMix(float v)
{
	/*
	v		0	.25	.5	.75	1.0
	mix0	0	1	.5	0	0	
	mix1	1	.5	0	.5	1	
	mix2	0	0	.5	1	0	
	*/
	mix = v;

	mix1 = 2 * fabs(0.5f - v);
	
	if( v < 0.25f )
	{
		mix0 = 4.0f * v;
		mix2 = 0.0f;
	}
	else if( v < 0.75f )
	{
		mix0 = 1.0f - 2.0f * (v - 0.25f);
		mix2 = 2 * (v - 0.25f);
	}
	else
	{
		mix0 = 0.0;
		mix2 = 1.0f - 4.0f * (v - 0.75f);
	}

	mix0 *= mix0;
	mix1 *= mix1;
	mix2 *= mix2;
	
	mix1 = fmax(0.25f, mix1);
	mix0 *= 2.0f;
	
	//cout << mix0 << "\t" << mix1 << "\t" << mix2 << "\n";
	
}

int Upright::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status)
{
	assert(outBuf != NULL);
	float **out = static_cast<float **>(outBuf);

	float sL;//, sR;
	uint p, p2, p3, frameClk;
	float o;

	for(int i=0; i < framesPerBuf; i++){
		sL = 0.0;// sR = 0.0;
		//if(!modvec){
		++clk;
		frameClk = clk % ADSR_DIVISOR;
		
		for(int n = lowNote; n <= highNote; n++)
		{
			noteData &note = notes[n];
			//note = notes[n];
			
			if(note.num != 0 ) //and frameClk == 0)
				doADSR(n);
			
			if(note.num != 0){

				p = note.phase;
				p2= note.phase2;
				p3= note.phase3;

				sL += keyfilt->iir_filter( n, note.amp * note.vel * // * note.vel *
					(
						mix1 * samples[n][0][p]     +
						mix0 * samples[n-12][0][p3] +
						mix2 * samples[n+12][0][p2]
					)
				);

				++(note.phase);
				++(note.phase2);
				++(note.phase3);
				
				if(note.phase >= SAMPLE_LENGTH) // non-looping samples
				{
					note.phase = 0;
					note.ampEnv.stage = '_';
				}
				if(note.phase2 > SAMPLE_LENGTH)
					note.phase2 = SAMPLE_LENGTH;
				if(note.phase3 > SAMPLE_LENGTH)
					note.phase3 = SAMPLE_LENGTH;
			}
		
		}

		o = applyEffects(sL);

		out[0][i] = o;
		out[1][i] = o;
		
	}
	

	return paContinue;
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

BrassLoader::BrassLoader(float ***s, bool *b) :
	wxThread(),
	samples(s),
	loaded(b)
{
}

void BrassLoader::loadSample( int i, wxString &fn)
{
	wxString filename = Oscillator::GetSamplePath();
	filename << fn;

	SndfileHandle *sf = new SndfileHandle(filename.mb_str(), SFM_READ);
	if( not sf->error() )
	{
		sf->read(samples[i][0], SAMPLE_LENGTH );
	} else exit(3);
	delete sf;
}

void BrassLoader::loadSample( int i, int j, double ratio )
{
	loaded[j] = true;
	// it's in memory, now to resample
	SRC_DATA sd;
	sd.data_in       = samples[   i    ][  0  ];
	sd.data_out      = samples[ j + 21 ][  0  ];
	sd.input_frames  = SAMPLE_LENGTH;
	sd.output_frames = SAMPLE_LENGTH;
	sd.src_ratio = 1./ ratio;
	src_simple(&sd, SRC_SINC_MEDIUM_QUALITY, 1);

	//keyfilt->set(i, 127);

}

void *BrassLoader::Entry(){

	int i = 32;
	wxString fn;
	Temperament *temper = new EquaTemp;

	fn = _T("Trump_A#2.wav"); loadSample( 0 , fn );
	for(; i <= 37; ++i) loadSample( 0 , i, (*temper)(i) / (*temper)(37));
	fn = _T("Trump_E3.wav" ); loadSample( 1 , fn );
	for(; i <= 43; ++i) loadSample( 1 , i, (*temper)(i) / (*temper)(43));
	fn = _T("Trump_G#3.wav"); loadSample( 2 , fn ); 
	for(; i <= 47; ++i) loadSample( 2 , i, (*temper)(i) / (*temper)(47));
	fn = _T("Trump_A#3.wav"); loadSample( 3 , fn );
	for(; i <= 49; ++i) loadSample( 3 , i, (*temper)(i) / (*temper)(49));
	fn = _T("Trump_C4.wav" ); loadSample( 4 , fn );
	for(; i <= 51; ++i) loadSample( 4 , i, (*temper)(i) / (*temper)(51));
	fn = _T("Trump_E4.wav" ); loadSample( 5 , fn );
	for(; i <= 55; ++i) loadSample( 5 , i, (*temper)(i) / (*temper)(55));
	fn = _T("Trump_F#4.wav"); loadSample( 6 , fn );
	for(; i <= 57; ++i) loadSample( 6 , i, (*temper)(i) / (*temper)(57));
	fn = _T("Trump_A#4.wav"); loadSample( 7 , fn );
	for(; i <= 61; ++i) loadSample( 7 , i, (*temper)(i) / (*temper)(61));
	fn = _T("Trump_C5.wav" ); loadSample( 8 , fn );
	for(; i <= 63; ++i) loadSample( 8 , i, (*temper)(i) / (*temper)(63));
	fn = _T("Trump_D5.wav" ); loadSample( 9 , fn );
	for(; i <= 65; ++i) loadSample( 9 , i, (*temper)(i) / (*temper)(65));
	
	delete temper;
	return NULL;
}

void BrassLoader::OnExit(){
}


///////////////////////////////////////////////////////////////////////////////

Brass::Brass()
{

	uint i;
	keyfilt->setRange(200., 10000.);
	
	//tableSize = 96000;
	for(i = 0; i < 128; ++i)
	{
		samples[i] = new float*[2];
		samples[i][0] = new float[SAMPLE_LENGTH];
		loaded[i] = false;
		//notes[i].freq = 1.0;
		//notes[i].phase *= 1;
		//notes[i].phase >>= 10;

		notes[i].phase >>= 10;
		notes[i].phase2 >>= 10;
		notes[i].phase3 >>= 10;
		/*
		notes[i].phase *= pitchbend;
		notes[i].phase2 *= pitchbend / 2;
		notes[i].phase3 *= pitchbend * 2;
		*/
	}
	
}

void Brass::loadSamples(){
	load = new BrassLoader(samples, loaded);
	load->Create();
	load->Run();	
}

Brass::~Brass()
{
/*
	for(int i = 0; i < 88; ++i)
	{
		delete samples[i][0];
		delete samples[i];
	}
	delete samples;
	delete keyfilt;
	//delete filt;
*/
}
