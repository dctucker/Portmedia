#include "myarduino.h"

MyArduino::MyArduino() : Arduino(), _sustain(false), down(0)
{

}

void MyArduino::Init(wxString &str)
{
	Arduino::Init(str.mb_str(), 115200);
}

void MyArduino::root(int n)
{
	down++;
	WriteByte( 1 + (n % 12) );
}

void MyArduino::off()
{
	down--;
	if( !_sustain && down <= 0 )
		WriteByte(0);
	else
		WriteByte('_');
}

void MyArduino::bend(float pitch)
{
	float x = 360 + pitch * 360.;
	int pit = (int)ceil(x);
	wxString p = wxString::Format(_T("%iB"), pit);
	//cout << p;
	Write(p.mb_str());
}

void MyArduino::filter(float pos)
{
	float x = pos * 2.;
	int po = (int)x;
	wxString p = wxString::Format(_T("%iW"), po);
	//cout << p;
	Write(p.mb_str());
}

void MyArduino::sustain(int sus)
{
	_sustain = sus != 0;
}



