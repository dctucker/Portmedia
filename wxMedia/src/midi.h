#ifndef MIDICALLBACK
#define MIDICALLBACK( X, Y, Z ) (double X, std::vector<unsigned char> * Y, void * Z)
#define MIDICB ((void)(double, std::vector<unsigned char> *, void *))
#endif

#ifndef MIDIMSG
#define MIDIMSG std::vector<unsigned char>*
#endif

#include <iostream>
#include <vector>

#ifndef MIDICTRLS
#define MIDICTRLS

class MidiController {
protected:
	void *data;
	double time;
public:
	void callback (double, MIDIMSG, void *);
	
	virtual void cc			(int ch,int c,int v)	{};
	virtual void noteOn		(int ch,int k,int v)	{};
	virtual void noteOff	(int ch,int k,int v)	{};
	virtual void aft		(int ch,int k,int p)	{};
	virtual void patch		(int ch,int p)			{};
	virtual void pressure	(int ch,int p)			{};
	virtual void bend		(int ch,int b)			{};
	virtual void sysex		(MIDIMSG)				{};
};

class BCR2000 : public MidiController {
protected:
	float *knob, *maxKnob, *minKnob;
	float *button;
	float *step;

public:
	BCR2000 (), ~BCR2000();
	void cc(int,int,int), noteOn(int,int,int), noteOff(int,int,int);
	virtual void turn(int, float);
	//virtual void on(int);
	//virtual void off(int);
	virtual void press(int,bool);
	float getKnob(int) const;
	float getButton(int) const;
	void setKnob(int, float);
};

#endif