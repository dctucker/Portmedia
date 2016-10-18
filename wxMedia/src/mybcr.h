#pragma once

#include "globals.h"
#include "mykeys.h"

class MyBCR : public BCR2000 { // BCR-2000 implementation
public:
	Canvas3D *canvas;
	//Oscillator *sub, *piano, *organ;
	MyKeys *myKeys;
	RtMidiOut *mo;
	std::vector<unsigned char> msg;
	bool loading;
	int inst;

	
	int curPage;
	float pages[5][128];
	
	Param *param[5][128];
	Set *set[5][128];

	MyBCR();

	#define SETCTRL( pg, kn, obj, prm ) \
		set[pg][kn] = obj; param[pg][kn] = obj-> FindParam( PN(prm) )
	
	static fl distParam (fl v){ return  1.0f - v * 0.9f; }
	static fl distParam1(fl y){ return (1.0f - y )/0.9f;  }
	
	friend ostream &operator<<(ostream &stream, MyBCR &bcr)
	{
		for(int p=0; p <= 4; ++p){
			for(int i=0; i < 128; ++i){
				if( i % 10 == 1 )
					stream << "\n";
				else
					stream << "\t";
				stream << bcr.pages[p][i];
			}
		}
		for(int i=0; i <= 98; ++i){
			if( i % 10 == 1 )
				stream << "\n";
			else
				stream << "\t";
			stream << bcr.button[i];
		}
		stream << "\n" << bcr.myKeys->loSplit << "\t" << bcr.myKeys->hiSplit;
		stream << "\n";
		return stream;
	}
	friend istream &operator>>(istream &stream, MyBCR &bcr)
	{
		for(int p=0; p <=4; ++p){
			for(int i=0; i < 128; ++i){
				stream >> bcr.pages[p][i];
			}
		}
		for(int i=0; i <= 98; ++i){
			stream >> bcr.button[i];
		}
		stream >> bcr.myKeys->loSplit;
		stream >> bcr.myKeys->hiSplit;
		bcr.myKeys->setLoRange( bcr.myKeys->loSplit );
		bcr.myKeys->setHiRange( bcr.myKeys->hiSplit );
		return stream;
	}

	void setControls();
	void send(unsigned char a, unsigned char b, unsigned char c);
	void sendValues();
	void execValues(bool = false);
	void update();
	void updatePage(int b);
	void press(int b, bool v);
	void turnPage( int p, int k, float v);
	void turn(int k, float v);
};
