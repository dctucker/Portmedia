#include "mykeys.h"

// KeyStation-88 implementation
MyKeys::MyKeys() : 
		loSplit(48), hiSplit(84), shelf(0.5),
		loSet(false), hiSet(false), inst(0),
		lastLP(1), subOn(true), subSus(false),
		canvas(NULL)
{
	for(int i = 0; i < NUM_OSCS; ++i ){
		hiAssign[i] = false;
		loAssign[i] = false;
	}
	//canvas->active[0] = subOn;

	//loAssign[2] = true;
	//hiAssign[3] = true;
}

void MyKeys::setOscs()
{
	for(int i=0; i < NUM_OSCS; i++)
	{
		oscs[i] = getOsc(i);
	}
	AddSet( sub,    "Sub" );
	AddSet( piano,  "Pian" );
	AddSet( organ,  "Hamm" );
	AddSet( reese,  "Rees" );
	AddSet( tri,    "Tria" );
	AddSet( square, "Squa" );
	AddSet( brass,  "Bras" );
	AddSet( noise,  "Nois" );

	AddParam( &loSplit, "LoSp" );
	AddParam( &hiSplit, "HiSp" );
	AddParam( &tuning,  "Tune" );
	AddParam( &shelf,   "Shlf" );
}

// note, for efficiency, these values are coded as longs instead of four-byte strings.
// Endianness is not preserved here.
void MyKeys::SetParam( int n, fl v ) 
{
	switch( n )
	{
		case 0x70536F4C:  // LoSp
			setLoRange( v );
			break;
		case 0x70536948:  // HiSp
			setHiRange( v );
			break;
		case 0x666C6853:  // Shlf
			setShelf( v );
			break;
	}
}

void MyKeys::setString(const char *s)
{
	canvas->keyOn(1);
}

void MyKeys::setTemper(int tun)
{
	Oscillator *osc = NULL;
	Temperament *t = Temperament::getTemp(tun);
	tuning = tun;
	for(int i=0; i < NUM_OSCS; ++i)
	{
		if( oscs[i] != NULL )
		{
			oscs[i]->setTemper(t);

			if( oscs[i]->loAssign or oscs[i]->hiAssign )
			{
				osc = oscs[i];
				/* // unstable
				if( osc->chord_ratio != 1.0 )
				{
					if( osc != NULL and canvas != NULL )
					{
						osc->calc_chord_ratio();
						canvas->chord_ratio = osc->chord_ratio;
					}
					
				}
				*/
			}
					
		}
	}
}

void MyKeys::duckSub()
{
	sub->duck();
}

void MyKeys::update()
{
	if(!updating)
	{
		updating = true;
		//panel->Refresh();
		//wxMouseEvent eve(wxEVT_MOTION);
		//::wxPostEvent(panel, eve);
		updating = false;
	}
	
}

	
void MyKeys::updateFilter(int ins)
{
	if( ins >= 0 )
	{
		Oscillator *osc = getOsc(ins);
		int i = 0;
		for(float f = 15.625f; f < 18000; f *= 2 )
		{
			for( int g = 0; g < 12 and i < 128; g++ ){
				float ef = f * powf(2., g / 12.f);
				float y = osc->queryFilter(ef);
				y += bus->queryFilter(ef);
				if( y < -36 ) y = -36;
				canvas->updateFilter(ins, i, y);
				//std::cout << i << " " << ef << "\t" << y << "\n";
				++i;
			}
		}
		for( ; i < 128; i++){
			canvas->updateFilter(ins, i, -36);
		}

		canvas->setBiquad( ins,
			osc->filtLP1->bq1.a0,
			osc->filtLP1->bq1.a1,
			osc->filtLP1->bq1.a2,
			osc->filtLP1->bq1.b0,
			osc->filtLP1->bq1.b1,
			osc->filtLP1->bq1.b2
		);
		//std::cout << "\n";
	} 
	else 
	{
		for(int i=7; i >= 0; i--)
		{
			if( i != inst )
				updateFilter(i);
		}
		updateFilter(inst);
	}
}

void MyKeys::updateEnvelope(int ins)
{
	//Oscillator *osc = getOsc(ins);
	/*
	canvas->updateEnvelope( ins, 
		osc->notes[0]->ampEnv->A,
		osc->notes[0]->ampEnv->D,
		osc->notes[0]->ampEnv->S,
		osc->notes[0]->ampEnv->R
	);
	*/
	
}

void MyKeys::setShelf(float f)
{
	shelf = f;
}

void MyKeys::setLoRange(int r)
{
	loSplit = r;
	canvas->loSplit = r;
}
void MyKeys::setHiRange(int r)
{
	hiSplit = r;
	canvas->hiSplit = r;
}


void MyKeys::noteOn(int ch, int k, int v)
{
	Oscillator *osc = NULL;
	if(k <= 20) // modulation keys
	{
		modulate(k);
	}		
	else if( k < loSplit ) // sub sine
	{
		if(subOn)
		{
			sub->mark(time);
			sub->noteOn(k, v);
		}
		
		if( oscs[1]->loAssign and k >= 21)
			piano->noteOn(k, v);
			
		if( hiSet )
		{
			modulate(k);
		}

		// activate other oscillators with shelf
		for(int i=0; i < NUM_OSCS; ++i)
		{
			if(oscs[i]->loAssign)
			{
				osc = oscs[i];
				osc->noteOn(k, v * shelf);
			}
		}

		// hook for lighting via serial port goes here
		arduino->root(k);
	}
	else if( k < hiSplit )
	{
		//osc->noteOn(k, v);
		//piano->noteOn(k,v);
		for(int i=0; i < NUM_OSCS; ++i)
		{
			if(oscs[i]->loAssign){
				osc = oscs[i];
				osc->noteOn(k,v);
			}
		}
	}
	else
	{
		for(int i=0; i < NUM_OSCS; ++i)
		{
			if( oscs[i]->hiAssign )
			{
				if(i == 1)
				{
					osc = piano;
					piano->noteOn(k,v);
				}
				else
				{
					osc = oscs[i];
					osc->noteOn(k - 24,v);
				}
			}
		}
	}

	canvas->keyOn(k);

	if(highNote < k) highNote = k;
	if(lowNote  > k) lowNote  = k;

	/* // unstable
	if( osc != NULL )
	{
		osc->calc_chord_ratio();
		canvas->chord_ratio = osc->chord_ratio;
		
	}
	//std::cout << "on " << osc->chord_ratio << "\n";
	//*/
	
	//inferChord();
	//update();
	
	//std::cout << k;
}

void MyKeys::noteOff(int ch, int k, int v)
{
	Oscillator *osc = NULL;
	canvas->keyOff(k);

	if( k < loSplit ) // lower range - sub
	{
		sub->mark(time);
		sub->noteOff(k);
		for(int i=0; i < NUM_OSCS; ++i){
			if( oscs[i]->loAssign )
				osc = oscs[i];
			oscs[i]->noteOff(k);
		}
		arduino->off();
	}
	else if( k < hiSplit ) // mid range
	{
		
		for(int i=0; i < NUM_OSCS; ++i)
		{
			if( oscs[i]->loAssign )
			{
				osc = oscs[i];
				oscs[i]->noteOff(k);
			}
		}
	}
	else // higher range
	{
		for(int i=0; i < NUM_OSCS; ++i)
		{
			if( oscs[i]->hiAssign )
			{
				if(i == 1)
				{
					osc = oscs[i];
					piano->noteOff(k);
				}
				else
				{
					osc = oscs[i];
					osc->noteOff(k - 24);
				}
			}
		}
	}

	if( loSet )
		setLoRange(k);
	
	else if( hiSet and k > loSplit )
		setHiRange(k);

	//osc->noteOff(k);
	//piano->noteOff(k);
	
	if(highNote == k)
		while( highNote >  0  and keyon[highNote] == 0 )
			highNote--;
	if(lowNote  == k)
		while( lowNote  < 127 and keyon[lowNote ] == 0 )
			lowNote++;

	
	///* // unstable
	if( osc != NULL ){
		osc->calc_chord_ratio();
		canvas->chord_ratio = osc->chord_ratio;
	}
	//*/
	
	//std::cout << "off " << osc->chord_ratio << "\n";

	update();
}

void MyKeys::cc(int ch, int c, int v)
{
	switch(c)
	{
		
		case 1: // mod wheel
		
			mod = (127. - v) / 127.;
			canvas->SetMod(mod);				
			bus->setFilter(mod);
			arduino->filter(127 - v);
			updateFilter(-1);
						
		break;
		
		case 7: // volume fader
		
			fader = v / 127.;
			canvas->SetFader(fader);				
			bus->setVol( fader );
		
		break;

		case 11: // expression controller

			//reese->setDetune( max(16,v) / 127. );

		break;
		
		case 64: // sustain pedal
		
			sustain = v / 128.;
			canvas->SetSustain(sustain);
			
			if(subSus) sub->sustain(v);
			for(int i=0; i < NUM_OSCS; ++i)
			{
				if( oscs[i]->loAssign or oscs[i]->hiAssign ){
					oscs[i]->sustain(v);
				}
			}
			arduino->sustain(v);
			
		break;
	}
	update();
	//osc->setCutoff((v+1) / 129.);
}

void MyKeys::bend(int ch, int b)
{
	pitch = (b - 8192) / 8192.;
	canvas->SetPitch(pitch);
	arduino->bend(pitch);
	
	oscs[0]->bend(pitch);
	for(int i = 0; i < NUM_OSCS; ++i){
		if( oscs[i]->loAssign or oscs[i]->hiAssign )
			oscs[i]->bend(pitch);
	}
	//organ->bend(pitch);
	//std::cout << "Bend: " << b << "\n";
	
	update();
}

void MyKeys::setDelay(float t)
{
	tempo = t;
	bpm = 60.f / tempo;

	for(int i=0; i < NUM_OSCS; i++)
	{
		//oscs[i]->setDelay(t);
		oscs[i]->delay->SetParam( PN("Time"), t * SAMPLE_RATE );
	}
	
	canvas->bpm = bpm;
}

void MyKeys::modulate( int k )
{
	int m = (k + 3) % 12;
	std::cout << "Modulating to " << m;
	oscs[0]->modulate(m);
	for(int i=1; i < NUM_OSCS; ++i){
		if( oscs[i] != NULL and (oscs[i]->loAssign or oscs[i]->hiAssign) )
			oscs[i]->modulate( m );
	}
}

void MyKeys::inferChord() // needs to be heavily optimized
{
	int i;

	int bass_note = -1;
	int note_count = 0;
	

	for(i=0; i < 12; i++) pitch_class[i] = 0;
	// find bass note and collect pitch class frequency values
	for(i = lowNote; i <= highNote; i++)
	{
		if( canvas->keyon[i] )
		{
			pitch_class[i % 12] += canvas->keyon[i] ? 1 : 0;
			if( bass_note == -1 )
			{
				bass_note = i % 12;
			}
			++note_count;
		}
	}
	
	if( note_count < 3 ) return;
	
	// determine major, minor, diminished, augmented
	for(i=0; i < 12; i++)
	{
		int m = (i + 3) % 12;
		int M = (i + 4) % 12;
		int T = (i + 6) % 12;
		int P = (i + 7) % 12;
		int A = (i + 8) % 12;
		
		minor[i] = pitch_class[i] * pitch_class[m] * pitch_class[P];
		major[i] = pitch_class[i] * pitch_class[M] * pitch_class[P];
		dimin[i] = pitch_class[i] * pitch_class[m] * pitch_class[T];
		augme[i] = pitch_class[i] * pitch_class[M] * pitch_class[A];
	}
	
	
	int pos = 0;
	for(i=0; pos < 10 && i < 12; i++)
	{
		if( major[i] > 0 )
		{
			poss[pos].type = 0;
			poss[pos].root = i;
			poss[pos].add1 = poss[pos].add2 = 0;
			poss[pos].weight = major[i];
			if( i == bass_note ) poss[pos].weight *= 2;
			++pos;
		}
		if( minor[i] > 0 )
		{
			poss[pos].type = 1;
			poss[pos].root = i;
			poss[pos].add1 = poss[pos].add2 = 0;
			poss[pos].weight = minor[i];
			if( i == bass_note ) poss[pos].weight *= 2;
			++pos;
		}
		if( dimin[i] > 0 )
		{
			poss[pos].type = 2;
			poss[pos].root = i;
			poss[pos].add1 = poss[pos].add2 = 0;
			poss[pos].weight = dimin[i];
			if( i == bass_note ) poss[pos].weight *= 2;
			++pos;
		}
		if( augme[i] > 0 )
		{
			poss[pos].type = 3;
			poss[pos].root = i;
			poss[pos].add1 = poss[pos].add2 = 0;
			poss[pos].weight = augme[i];
			if( i == bass_note ) poss[pos].weight *= 2;
			++pos;
		}
	}
	
	int maxi = 0, maxw = 0;
	for(i = 0; i < pos; i++)
	{
		if( poss[i].weight > maxw )
		{
			maxw = poss[i].weight;
			maxi = i;
			
		}
	}
	
	i = maxi;

	int r = poss[i].root;
	int
		a9  = pitch_class[( r +  2 ) % 12],
		sus = pitch_class[( r +  5 ) % 12],
		m6  = pitch_class[( r +  8 ) % 12],
		M6  = pitch_class[( r +  9 ) % 12],
		m7  = pitch_class[( r + 10 ) % 12],
		M7  = pitch_class[( r + 11 ) % 12];

	if( a9 > 0 )
		poss[i].add1 = 9;
	
	else if( sus > 0 )
		poss[pos].add1 = 4;
	
	if( ( m6 > 0 or M6 > 0 ) and ( poss[i].type != 3 ) )
		poss[i].add2 = 6;
	
	else if( m7 > 0 or M7 > 0 )
		poss[i].add2 = 7;

	poss[i].bass = bass_note;
	canvas->curChord = poss[i];
}	

