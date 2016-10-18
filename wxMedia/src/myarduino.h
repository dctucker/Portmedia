#ifndef MYARDUINO_H
#define MYARDUINO_H

#include "globals.h"

// lights controller
class MyArduino : public Arduino
{
private:
	bool _sustain;
	int down;
public:
	MyArduino();
	void Init(wxString &str);
	void root(int n);
	void off();
	void bend(float pitch);
	void filter(float pos);
	void sustain(int sus);
};

#endif
