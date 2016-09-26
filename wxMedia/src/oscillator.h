#ifndef OSCILLATOR_H
#define OSCILLATOR_H

//#include <omp.h>
//#include <vector>

//#include <map>
#include <iostream>
#include "wx/app.h"

#include <portaudiocpp/PortAudioCpp.hxx>

#include "temperament.h"
#include "ladder.h"
#include "biquad.h"
#include "delay.h"

#include "param.h"
//#include "bus.h"
#include "adsr.h"
#include "pan.h"

#ifdef WIN32
	#define min(a,b) ( ( (a) < (b) ) ? (a) : (b) )
	#define max(a,b) ( ( (a) > (b) ) ? (a) : (b) )

	static inline float fmaxf(float a, float b) 
 	{ 
 	        unsigned long NaN[2]={0xffffffff, 0x7fffffff}; 
 	        double nan = *( double* )NaN; 
 	 
 	        if( _isnan(a) && _isnan(b)) return nan; 
 	 
 	        return ( max(a, b || _isnan(b) ?  a : b )); 
 	} 
 	static inline float fminf(float a, float b) 
 	{ 
 	        unsigned long NaN[2]={0xffffffff, 0x7fffffff}; 
 	        double nan = *( double* )NaN; 
 	 
 	        if( _isnan(a) && _isnan(b)) return nan; 
 	 
 	        return (min(a, b || _isnan(b) ? a : b )); 
 	} 
#endif  // WIN32


#define FL_DBFS( F ) 20 * log ( F )
#define DBFS_FL( L ) pow( 10., (L / 20.) )

#ifndef M_PI
#define M_PI   3.141592653589793
//#define M_PI 3.14159265358979323846
//             3.14159265
#endif


#define maxPoly 128
#define GETSET( T, X, Y ) T get##X () { return Y; } void set##X (T a) { Y = a; }
#define GENERATE(X,Y,Z,W,S) ( const void * (X), void * (Y), unsigned long (Z), const PaStreamCallbackTimeInfo *(W), PaStreamCallbackFlags (S) )

typedef float fl;

inline float faderVol(int v){
	float r;
	if( v >= 32 )
		r = 36.f * ( (1.f - (v - 32) / 95.f ) );
	else if( v >= 1 )
		r = 36.f + 2 * (32 - v);
	else
		return 0.0f;
		
	return powf(10, -r/20.f);
}



class Generator {
public:
	bool active;
	virtual int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status) = 0;

};



class Waveform
{
public:
	virtual fl operator() (noteData &) {return 0;}
};

class WaveTable 
	: public Waveform
{
public:
	WaveTable();
	~WaveTable(){
		delete table;
	}
	
	fl operator() (noteData &);
	
	fl * GetTable(){ return table; };
	void setupTable(){ table = new fl[tableSize + 1]; }
	int tableSize;
	fl *table;
protected:
	int tableFactor;

	
};


class VariWave : public Waveform 
{
public:
	fl x[6], y[6], m[6];
	fl wave, w, h, v;

	VariWave();
	fl operator() (noteData &);
	fl func( fl t );
	void shape();
};

class Oscillator : public Generator, public Set
{
public:
	Filter *filtLP1, *filtHP1;
	Filter *filtLP2, *filtHP2;
	Delay *delay;
	Pan *pan;

	unsigned int clk;
	//FilterIIR *filtLP;
	//butterworth *filtHP;
	
	
	fl amp, pitchbend, bendInt;
	fl dist, fLP, fHP, qLP, qHP;
	
	// convert the above to integers for performance increase.
	//fl operator() (noteData &);

	WaveTable *lfo;
	noteData lfoNote;
	

	Waveform *wf;
	
	//fl lowpass, highpass;
	Temperament *temper;
	bool modvec;
	int sus, poly;
	int highNote, lowNote, curNote;
	int hiAssign, loAssign;
	fl lfof;
	fl lfo2lp, lfo2hp, lfo2amp, env2lp;
	
public:
	noteData *notes;

	float chord_ratio;
	void calc_chord_ratio();

	Oscillator();	
	~Oscillator(){
		//delete temper;
		delete notes;
		delete pan;
	}

	virtual void SetParam( int n, fl v );

	void setTemper(Temperament *);
	virtual void bend(fl);
	virtual void retemper();
	virtual void retune(fl);
	virtual void modulate(int);
	
	void setDist(fl);
	void setDelay(fl t=-1, fl f=-1);
	void setLFO(fl a, fl f=-1);
	void setLowpass (fl,fl q=-1);
	void setHighpass(fl,fl q=-1);
	void setLFO2LP(fl v);
	void setLFO2HP(fl v);
	void setLFO2Amp(fl v);
	void setEnv2LP(fl v);
	
	fl queryFilter(fl f);

	GETSET( fl, Amp, amp );
	//GETSET( fl, Cutoff, cutoff );

	fl aA,aD,aS,aR,fA,fD,fS,fR;
	
	void doADSR(int);
	void setADSR       (fl a=-1., fl d=-1., fl s=-1., fl r=-1.);
	void setFilterADSR (fl a=-1., fl d=-1., fl s=-1., fl r=-1.);
	//void setA(fl){}

	virtual void noteOn(int n=-1, int v=-1);
	void noteOff(int n);
	void sustain(int v);

	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);
	fl applyEffects(fl);
	virtual void analog(){}
	
	static wxString GetExecutablePath(), GetSamplePath();
	
	int calcFreq(int);
	int calcFreq(fl);

};

#define MULTI( X ) Oscillator:: X ; for(int i=0; i < numOscs; i++){ \
	if(oscs[i] != NULL) oscs[i]-> X; }

class MultiOsc : public Oscillator
{
	int numOscs;
	Oscillator **oscs;
	MultiOsc(){
		numOscs = 0;
		oscs = new Oscillator *[128];
	}
	void add(Oscillator *o){
		oscs[numOscs++] = o;
	}
	void setADSR(fl a, fl d, fl s, fl r){
		MULTI( setADSR(a, d, s, r) );
	}
	void noteOn(int n, int v){
		MULTI( noteOn(n, v) );
	}
	void noteOff(int n){
		MULTI( noteOff(n) );
	}
	void doADSR(int k){
		MULTI( doADSR(k) );
	}
	void setTemper(Temperament *t){
		MULTI( setTemper(t) );
	}
};

#define OSCSUB( Y ) \
	class Y : public Oscillator { public: Y(); };
	
#define OSCTBL( X , Y) \
	class X : public WaveTable { public: X(); }; \
	class Y : public Oscillator, public X { public: Y(){ wf = new X(); } };

OSCTBL( SineTbl,     SineOsc )
OSCTBL( TriangleTbl, TriangleOsc )
OSCTBL( SquareTbl,   SquareOsc  )
OSCTBL( SawTbl,      SawOsc )
OSCTBL( NoiseTbl,    NoiseOsc )

#endif
