#include "mybcr.h"

MyBCR::MyBCR() : 
	mo(NULL),
	myKeys(NULL),
	curPage(1),
	loading(false),
	canvas(NULL)
{
	
	for(int i=0; i < 128; i++)
	{
		for(int j=0; j < 5; j++)
		{
			pages[j][i] = 0.0;
			param[j][i] = NULL;
		}
		button[i] = 0.0;
	}

	msg.push_back(0x00);
	msg.push_back(0x00);
	msg.push_back(0x00);
	
}

void MyBCR::setControls()
{
	SETCTRL( 4, 21, myKeys, "Shlf"  );
	SETCTRL( 4, 22, myKeys->piano, "Mix"  );
	SETCTRL( 4, 24, myKeys->reese, "Detu" );
	SETCTRL( 4, 27, myKeys->brass, "Mix"  );

	for( int i=0 ; i < NUM_OSCS ; i++ )
	{
		SETCTRL(0, 81+i, myKeys->oscs[i], "HiAs" );
		SETCTRL(0, 91+i, myKeys->oscs[i], "LoAs" );
		
		SETCTRL(1, 11+i, myKeys->oscs[i]->filtLP1, "Freq" );
		SETCTRL(1, 21+i, myKeys->oscs[i]->filtHP1, "Freq" );
		SETCTRL(1, 31+i, myKeys->oscs[i], "AmpV" );
		
		SETCTRL(2, 11+i, myKeys->oscs[i]->filtLP1, "Reso" );
		SETCTRL(2, 21+i, myKeys->oscs[i]->filtHP1, "Reso" );
		SETCTRL(2, 31+i, myKeys->oscs[i], "EnLP" );
		
		SETCTRL(3, 11+i, myKeys->oscs[i], "LFLP" );
		SETCTRL(3, 21+i, myKeys->oscs[i], "LFOF" );
		SETCTRL(3, 31+i, myKeys->oscs[i], "LFOA" );
		
		SETCTRL(4, 11+i, myKeys->oscs[i]->delay, "Fdbk" );
		//TCTRL(4, 21+i, myKeys->oscs[i], "Mix" );
		SETCTRL(4, 31+i, myKeys->oscs[i], "Dist" );
		
		SETCTRL(1, 41+i, myKeys->oscs[i], "AmpA" );
		SETCTRL(1, 51+i, myKeys->oscs[i], "AmpD" );
		SETCTRL(1, 61+i, myKeys->oscs[i], "AmpS" );
		SETCTRL(1, 71+i, myKeys->oscs[i], "AmpR" );
		
		SETCTRL(2, 41+i, myKeys->oscs[i], "FltA" );
		SETCTRL(2, 51+i, myKeys->oscs[i], "FltD" );
		SETCTRL(2, 61+i, myKeys->oscs[i], "FltS" );
		SETCTRL(2, 71+i, myKeys->oscs[i], "FltR" );
		
		SETCTRL(4, 41+i, myKeys->oscs[i]->pan, "PanB" );

		PARAMFUNC( myKeys->oscs[i], "AmpV", Delay::square, Delay::root );
		PARAMFUNC( myKeys->oscs[i], "Dist", distParam, distParam1 );
		PARAMFUNC( myKeys->oscs[i]->delay, "Fdbk", Delay::square, Delay::root);
		PARAMFUNC( myKeys->oscs[i]->filtLP1, "Freq", Filter::lpFreq, Filter::lpFreq1 );
		PARAMFUNC( myKeys->oscs[i]->filtHP1, "Freq", Filter::hpFreq, Filter::hpFreq1 );
		PARAMFUNC( myKeys->oscs[i]->filtLP1, "Reso", Filter::lpRes , Filter::lpRes1 );
		PARAMFUNC( myKeys->oscs[i]->filtHP1, "Reso", Filter::hpRes , Filter::hpRes1 );
	}
}
	
void MyBCR::send(unsigned char a, unsigned char b, unsigned char c)
{
	msg[0] = a;
	msg[1] = b;
	msg[2] = c;
	if( mo != NULL ) mo->sendMessage(&msg);		
}

void MyBCR::sendValues()
{
	if( mo )
	{
		for(int i=40; i < 128; ++i)
		{
			send(0xBF, i, 127 * knob[i]);
		}
		for(int i=0; i <= 98; ++i)
		{
			send(0x9F, i, button[i] ? 0x7F : 0x00);
		}
	}
}

void MyBCR::execValues(bool is_loading)
{
	loading = is_loading;
	
	for(int p=0;  p <=4; p++)
	{
		for(int i=0; i < 128; i++)
		{
			turnPage(p, i, pages[p][i]);
		}
	}

	
	for(int i=0; i <= 98; ++i){
		press(i, button[i]);
	}

	for(int i=0; i < NUM_OSCS; ++i){
		myKeys->canvas->active[i] = 1;
	}
	
	loading = false;
}

void MyBCR::update()
{
	//if(panel) panel->Refresh();
	//wxMouseEvent eve(wxEVT_MOTION);
	//::wxPostEvent(panel, eve);
}

void MyBCR::updatePage(int b)
{
	curPage = b - 100;

	//myKeys->canvas->doMessage( pageNames[curPage] );
	
	//activate the proper light
	for(int i = 1; i <= 4; i++)
	{
		if(i == curPage)
		{
			send( 0x9F, b, 0x7F );
		}
		else
		{
			send( 0x9F, 100 + i, 0x00 );
		}
	}
	
	// set and send knob assignments for page
	Param *pm;
	float v;
	for(int i = 0; i < 128; ++i)
	{
		pm = param[curPage][i];
		if( pm != NULL )
		{
			v = pm->GetValue();
			if( pm->func != NULL )
				v = pm->func1(v);
			pages[curPage][i] = v;
		}
		else
		{
			v = pages[curPage][i];
		}
		knob[i] = v;
		send( 0xBF, i, 0x7F * v );
	}
	for(int i=81; i < 99; i++)
	{
		pm = param[0][i];
		if( pm != NULL )
		{
			v = pm->GetValue();
			button[i] = v;
			send( 0x9F, i, 0x7F * v );
		}
	}
}

void MyBCR::press(int b, bool v)
{
	/*
	char *pageNames[] =
	{
		"MAIN",
		"RES",
		"LFO",
		"FX"
	};
	*/
	
	int inst = ( b - 1 ) % 10;
	switch(b)
	{
		// preset selection
		case 51: case 52: case 53: case 54: case 55: case 56: case 57: case 58:
			if( v > 0.5)
			{
				wxCommandEvent menue( wxEVT_COMMAND_MENU_SELECTED, 1 + LOADID + inst );
				::wxPostEvent( wxTheApp, menue );
			}
			break;
			
		// tuning activation
		case 61: case 62: case 63: case 64: case 65: case 66: case 67: case 68:
			if(v > 0.5)
			{
				wxCommandEvent menue( wxEVT_COMMAND_MENU_SELECTED, TUNEID + b - 61 );
				::wxPostEvent( wxTheApp, menue );
			}
			break;

		case 41:
			myKeys->subSus = 1.0;//(v > 0.5 );
			break;
		
		case 42:
			if(v == 1.0) myKeys->piano->loadSamples();
			break;
			
		case 47:
			if(v == 1.0) myKeys->brass->loadSamples();
			break;
		
		// manual assignments
		case 81: // set hiSplit
			myKeys->hiSet = v;
			//myKeys->SetParam( PN("HiSp"), v );
			break;
		
		case 91: // set loSplit
			myKeys->loSet = v;
			//myKeys->SetParam( PN("LoSp"), v );
			break;
		
		case 82: case 83: case 84: case 85: case 86: case 87: case 88:
			myKeys->hiAssign[inst] = v;
			myKeys->oscs[inst]->hiAssign = v;
			myKeys->canvas->active[inst] = 
				myKeys->loAssign[inst] | 
				myKeys->hiAssign[inst] ;
			myKeys->canvas->selinst = inst;
			break;
			
		case 92: case 93: case 94: case 95: case 96: case 97: case 98:
			myKeys->loAssign[inst] = v;
			myKeys->oscs[inst]->loAssign = v;
			myKeys->canvas->active[inst] = 
				myKeys->loAssign[inst] | myKeys->hiAssign[inst] ;
			myKeys->canvas->selinst = inst;
			break;
			
		// mode selection
		case 104: // effects mode
		
			// tap tempo
			if( curPage == 4 and time < 2.0 )
			{
				myKeys->setDelay(time);
			}
			updatePage(b);
			break;
			
		case 101: // amplitude mode
			updatePage(b);
			//updateFilter(0);
			break;
			
		case 102: // frequency mode
		case 103: // LFO mode
			updatePage(b);
			break;

		// pedals
		case 111:
			myKeys->subSus = v;//(v > 0.5 );
			break;
		case 112:
			myKeys->subOn = v;//( v > 0.5 );
			myKeys->canvas->active[0] = myKeys->subOn;
			break;
	}
	//std::cout << b << "\n";
	//osc->noteOn(b, 100);
	update();
}


void MyBCR::turnPage( int p, int k, float v)
{

	const char *instNames[] =
	{
		"BASS ",
		"PIANO",
		"ORGAN",
		"REESE",
		"ANGLE",
		"PULSE",
		"BRASS",
		"NOISE"
	};

	const char *names[][7] = {
	{
		"","","",
		"","","",""
	},{
		"LPF",
		"HPF",
		"VOL",
		"A-A","A-D","A-S","A-R"
	},{
		"LPQ",
		"HPQ",
		"ENV-LPF",
		"F-A","F-D","F-S","F-R"
	},{
		"LFO-LPF",
		"LFO RATE",
		"LFO-AMP",
		"","","",""
	},{
		"DELAY",
		"MOD",
		"DIST",
		"PAN","","",""
	}};
	
	//float f;
	inst = (k - 1) % 10;
	myKeys->inst = inst;
	pages[p][k] = v;
	
	int ks = (k / 10) - 1;

	Param *pm = param[p][k];
	if( pm != NULL )
	{
		if( pm->func != NULL )
			v = pm->func(v);
		set[p][k]->SetParam( pm->name.v, v );
	}
	
	if( ( p == 1 or p == 2 ) and k > 10 and k < 29 and inst < 8 )
	{
		if( not loading )
			myKeys->updateFilter( inst );
	}
	
	
	if( ks >= 0 and ks < 7 and inst >=0 and inst < 8)
	{
		if( not loading )
		{
			wxString str(instNames[inst], wxConvUTF8);
			wxString str1(names[p][ks], wxConvUTF8);
			str << _T(" ");
			str << str1;
			str << _T(": ");
			str << v;

			myKeys->canvas->doMessage( str.mb_str(wxConvUTF8) );
			if( pm ) myKeys->canvas->SetParam(inst, pm->name.v, v);
		}
	}

}

void MyBCR::turn(int k, float v)
{
	turnPage(curPage, k, v);
	canvas->turn(k, v);
	update();
}
