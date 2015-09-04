#ifndef OSCILLATOR_H
#define OSCILLATOR_H

//#include <omp.h>
//#include <vector>

//#include <map>
#include <iostream>
#include "temperament.h"
//#include "filter.h"
//#include "butterworth2.h"
#include "biquad.h"

#include <wx/app.h>


#define FL_DBFS( F ) 20 * log ( F )
#define DBFS_FL( L ) pow( 10., (L / 20.) )

#ifndef M_PI
#define M_PI   3.141592653589793
//#define M_PI 3.14159265358979323846
//             3.14159265
#endif


#define SIZE 524288
#define MASK 0x7FFFF
//#define resolution 524288.
#define resolution 4294967295.L
#define SHIFT (32-19)

#define ADSR_DIVISOR (1)
#define ADSR_MASK    (0x0)
// 16, 0xF

// SHIFT is defined as the bit-difference between the table size and the resolution
//#include "bus.h"
#include "delay.h"
#include "portaudiocpp/PortAudioCpp.hxx"


#define GETSET( T, X, Y ) T get##X () { return Y; } void set##X (T a) { Y = a; }

#define maxPoly 128

#define GENERATE(X,Y,Z,W,S) ( const void * (X), void * (Y), unsigned long (Z), const PaStreamCallbackTimeInfo *(W), PaStreamCallbackFlags (S))

class Generator {
public:
	bool active;
	virtual int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status) = 0;

};


typedef float fl;


class Counter {
protected:
	uint phase;
	uint incr;
	uint bits;
	//static const uint bits = SHIFT;
	
public:
	Counter(uint b = 0, uint incr = 1) :
		phase(0), incr(incr), bits(SHIFT)
	{
	}
	void operator *= (uint i){ // change frequency
		incr = i;
	}
	void operator >>= (uint i){ // change shift amount
		bits = i;
	}
	void operator ++ () { // increment prefix
		phase += incr;
	}
	uint operator ++ (int){ // increment postfix
		uint r = phase;
		phase += incr;
		return r >> bits;
	}
	void operator += (uint i) { // manual increment
		phase += i;
	}
	void operator = (uint i){ // reset phase
		phase = i << bits;
	}
	operator uint () {
		return phase >> bits;
	}
	//*/
};


class ADSR {
public:
	char stage;

protected:
	fl A, D, S, R;
	fl dA, dD, dS, dR;
	uint tA, tD, tR, clk;
	fl val;

public:
	ADSR(fl a=0.1, fl d=1.0, fl s=0.75, fl r=0.25) :
		A(a), D(d), S(s), R(r), val(0.0f)
	{
		calcADSR();
	}
	void calcADSR();
	void setADSR (fl a=-1., fl d=-1., fl s=-1., fl r=-1.);
	fl doADSR();
};

typedef struct {
	int num;
	bool down;
	//int stage;
	ADSR ampEnv, fltEnv;
	Counter phase, phase2;
	//unsigned int phase,phase2;
	unsigned int freq;
	//unsigned int time;
	fl amp;
	fl vel;
} noteData;


class WaveTable
{
public:
	WaveTable();
	~WaveTable(){
		delete table;
	}
	
	fl operator() (noteData &);

	
	fl * GetTable(){ return table; };
	void setupTable(){ table = new fl[tableSize]; }
	int tableSize;
	fl *table;
protected:
	int tableFactor;

	
};

class Oscillator : public Generator { //, public WaveTable {
protected:
	unsigned int clk;
	//FilterIIR *filtLP;
	//butterworth *filtHP;
	BiQuad *filtLP, *filtHP;
	Delay *delay;
	
	fl amp, pitchbend, bendInt;
	fl dist, fLP, fHP, qLP, qHP;
	
	// convert the above to integers for performance increase.
	//fl operator() (noteData &);

	WaveTable *lfo;
	noteData lfoNote;
	

	WaveTable *wt;
	
	//fl lowpass, highpass;
	Temperament *temper;
	bool modvec;
	int sus, poly;
	int highNote, lowNote;
	noteData *notes;
	
	fl lfo2lp, lfo2hp, lfo2amp;
	
public:
	Oscillator();	
	~Oscillator(){
		//delete temper;
		delete notes;
	}

	void setTemper(Temperament *t){
		//if(temper != NULL) delete temper;
		temper = t;
		retemper();
	}
	void bend(fl);
	virtual void retemper();
	void retune(fl);
	void modulate(int);
	
	void setDist(fl);
	void setDelay(fl t=-1, fl f=-1);
	void setLFO(fl a,fl f=-1);
	void setLowpass (fl,fl q=-1);
	void setHighpass(fl,fl q=-1);
	void setLFO2LP(fl v);
	void setLFO2HP(fl v);
	void setLFO2Amp(fl v);

	GETSET( fl, Amp, amp );
	//GETSET( fl, Cutoff, cutoff );

	void doADSR(int);
	void setADSR (fl a=-1., fl d=-1., fl s=-1., fl r=-1.);

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

#define MULTI( X ) Oscillator:: X ; for(int i=0; i < numOscs; i++){ if(oscs[i] != NULL) oscs[i]-> X; }

class MultiOsc : public Oscillator {
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
	class Y : public Oscillator, public X { public: Y(){ wt = new X(); } };

OSCTBL( SineTbl,     SineOsc )
OSCTBL( TriangleTbl, TriangleOsc )
OSCTBL( SquareTbl,   SquareOsc  )
OSCTBL( SawTbl,      SawOsc )
OSCTBL( NoiseTbl,    NoiseOsc )

#endif
