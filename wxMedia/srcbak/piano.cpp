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

	wxString filename = Oscillator::GetSamplePath();
	filename << _T("_") << (i + 21) << _T(".aif.wav");

	SndfileHandle *sf = new SndfileHandle(filename.mb_str(), SFM_READ);

	if( not sf->error() )
	{
		sf->read(samples[i % 21][0], SAMPLE_LENGTH );

		// it's in memory, now to resample
		double ratio = pow( 2, ( 30. + retune[i] ) / 1200. );
		SRC_DATA sd;
		sd.data_in       = samples[ i % 21 ][  0  ];
		sd.data_out      = samples[ i + 21 ][  0  ];
		sd.input_frames  = SAMPLE_LENGTH;
		sd.output_frames = SAMPLE_LENGTH;
		sd.src_ratio = 1./ ratio;
		src_simple(&sd, SRC_SINC_MEDIUM_QUALITY, 1);
		
		// finally, apply attenuation
		float att = powf( 10, 0.05f * ( 3.0 + atten[i] ) );
		for(int j=0; j < SAMPLE_LENGTH; ++j){
			samples[ i + 21][0][j] *= att;
		}

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
	keyfilt( new FilterIIR77 )
{

	//wt = new WaveTable();
	keyfilt->setRange(200., 10000.);
	//notes = wt->notes;
	
	//INIT_PD;
		
	//pd.Pulse(_T("Allocating memory"));

	tableSize = 96000;
	for(int i = 0; i < 128; ++i)
	{
		samples[i] = new float*[2];
		samples[i][0] = new float[SAMPLE_LENGTH];
		loaded[i] = false;
		notes[i].freq = 1.0;
		notes[i].phase >>= 0;
		notes[i].phase *= 1;
	}

	/*
	for(int i=22; i < 44; ++i) // 0 to 88 normally
	{
		//keyfilt[i + 21] = new FilterIIR;
		loadSample(i);
		if( not pd.Update( i ) ){
			exit(1);   // since it's loaded at startup, abort if requested
		}
	}*/
	
	//pd->Hide();
	//delete pd;
}


Upright::~Upright()
{
	for(int i = 0; i < 88; ++i)
	{
		delete samples[i][0];
		delete samples[i];
	}
	delete samples;
	delete keyfilt;
	//delete filt;
}

void Upright::loadSamples()
{
	/*
	//#pragma omp parallel for
	for(int i =0; i < 88; ++i){
		if( not loaded[i] )
			loadSample( i );
	}*/
	load = new UprightLoader(samples, loaded);
	load->Create();
	load->Run();
}

void Upright::loadSample( int i )
{
/*
	loaded[i] = true;

	
	
	keyfilt->set(i, 127);
	//pd->Hide();
*/
}


void Upright::noteOn(int n, int v)
{
	//int t = notes[n].time;
	if( not loaded[n - 21] )
		this->loadSample( n - 21 );
	else {
		//A = 0.1 - (0.1)*((v*v)/128.);
		Oscillator::noteOn(n,v);
		notes[n].phase = 0;
		notes[n].freq = 1.0;
		keyfilt->set(n,v);
	}
}

void Upright::setFilter(double fc){
	//filtLP->set(fc, 0.7);
}


void Upright::bend(float b){
	pitchbend = powf(2, bendInt * b / 12. );
	Upright::retemper();
}

void Upright::retemper() {
	for(int n=0; n < 128; n++){
		if(notes[n].num != 0){
			notes[n].freq = /*((*temper)(n) / tet(n)) * */ pitchbend;
		}
	}
}



int Upright::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status) {
	assert(outBuf != NULL);
	float **out = static_cast<float **>(outBuf);

	float sL;//, sR;
	uint p, f, frameClk; // consider using doubles or floats here.
	float o;
	//noteData &note = notes[0];

	//float filt[2][SAMPLE_RATE];// = new float[2][SAMPLE_RATE];

	for(int i=0; i < framesPerBuf; i++){
		sL = 0.0;// sR = 0.0;
		//if(!modvec){
		++clk;
		frameClk = clk % ADSR_DIVISOR;
		
		for(int n = lowNote; n <= highNote; n++){
			noteData &note = notes[n];
			//note = notes[n];
			
			if(note.num != 0 ) //and frameClk == 0)
				doADSR(n);
			
			if(note.num != 0){
				//note.time++;

				p = note.phase;
				f = note.freq;

				sL += keyfilt->iir_filter( n,
					samples[n][0][p] * note.amp * note.vel * note.vel
				);
				//sR += samples[n][0][p] * notes[n].amp * notes[n].vel;

				note.phase += f; //f; //f should be 1 most of the time.
				if(note.phase >= SAMPLE_LENGTH)
				{
					note.phase = 0;
					note.ampEnv.stage = '_';
				}
			}
		//}
		
		}
		//float o = filtLP->iir_filter( sL * amp );
		o = applyEffects(sL);
		o *= amp;
		out[0][i] = o;
		out[1][i] = o;
		
	}
	

	return paContinue;
}

///////////////////////////////////////////////////////////////////////////////

Brass::Brass() {

	uint i;
	keyfilt->setRange(200., 10000.);
	
	//INIT_PD;
		
	//pd.Pulse(_T("Allocating memory"));

	tableSize = 96000;
	for(i = 0; i < 128; ++i)
	{
		samples[i] = new float*[2];
		samples[i][0] = new float[SAMPLE_LENGTH];
		loaded[i] = false;
		notes[i].freq = 1.0;
		notes[i].phase *= 1;
		notes[i].phase >>= 0;
	}

	/*
	for(int i=22; i < 44; ++i) // 0 to 88 normally
	{
		//keyfilt[i + 21] = new FilterIIR;
		loadSample(i);
		if( not pd.Update( i ) ){
			exit(1);   // since it's loaded at startup, abort if requested
		}
	}*/
	
	//pd->Hide();
	//delete pd;
	
}


void Brass::loadSamples(){

	int i = 32;
	wxString filename;

	filename = _T("Trump_A#2.wav"); loadSample(0, filename);
	filename = _T("Trump_E3.wav" ); loadSample(1, filename);
	filename = _T("Trump_G#3.wav"); loadSample(2, filename); 
	filename = _T("Trump_A#3.wav"); loadSample(3, filename);
	filename = _T("Trump_C4.wav" ); loadSample(4, filename);
	filename = _T("Trump_E4.wav" ); loadSample(5, filename);
	filename = _T("Trump_F#4.wav"); loadSample(6, filename);
	filename = _T("Trump_A#4.wav"); loadSample(7, filename);
	filename = _T("Trump_C5.wav" ); loadSample(8, filename);
	filename = _T("Trump_D5.wav" ); loadSample(9, filename);
	
	for(; i <= 37; ++i) loadSample(0, i, (*temper)(i) / (*temper)(37));
	for(; i <= 43; ++i) loadSample(1, i, (*temper)(i) / (*temper)(43));
	for(; i <= 47; ++i) loadSample(2, i, (*temper)(i) / (*temper)(47));
	for(; i <= 49; ++i) loadSample(3, i, (*temper)(i) / (*temper)(49));
	for(; i <= 51; ++i) loadSample(4, i, (*temper)(i) / (*temper)(51));
	for(; i <= 55; ++i) loadSample(5, i, (*temper)(i) / (*temper)(55));
	for(; i <= 57; ++i) loadSample(6, i, (*temper)(i) / (*temper)(57));
	for(; i <= 61; ++i) loadSample(7, i, (*temper)(i) / (*temper)(61));
	for(; i <= 63; ++i) loadSample(8, i, (*temper)(i) / (*temper)(63));
	for(; i <= 65; ++i) loadSample(9, i, (*temper)(i) / (*temper)(65));
}

Brass::~Brass()
{
	for(int i = 0; i < 88; ++i)
	{
		delete samples[i][0];
		delete samples[i];
	}
	delete samples;
	delete keyfilt;
	//delete filt;
}

void Brass::loadSample(int i){
	return;
}

void Brass::loadSample( int i, wxString &fn)
{
	wxString filename = GetSamplePath();
	filename << fn;

	SndfileHandle *sf = new SndfileHandle(filename.mb_str(), SFM_READ);
	if( not sf->error() )
	{
		sf->read(samples[i][0], SAMPLE_LENGTH );
	} else exit(3);
	delete sf;
}

void Brass::loadSample( int i, int j, double ratio )
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

	keyfilt->set(i, 127);

}
