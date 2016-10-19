
#include "oscillator.h"

//inline int max(int a, int b){ return a > b ? a : b; }
//inline int min(int a, int b){ return a < b ? a : b; }
//inline float max(float a, float b){ return a < b ? a : b; }
//inline float min(float a, float b){ return a > b ? a : b; }


///////////////////////////////////////////////////////////////////////////

Oscillator::Oscillator() : 
	modvec(true), sus(0), dist(0.0),
	amp(0.25), poly(0), 
	pitchbend(1.0), bendInt(12.0), highNote(0), lowNote(127), clk(0),
	lfo2lp(0), lfo2hp(0), lfo2amp(0.0), curNote(0),
	chord_ratio(1.0)
{
	//calcADSR();
	
	temper = new EquaTemp();
	//wt->setupTable();
	//notes = wt->notes;

	notes = new noteData[maxPoly];
	for(int n=0; n < maxPoly; n++) // all notes off
	{
		noteData note;
		note.num    = 0;
		//note.freq   = 440.0;
		note.phase  = 0;
		note.phase2 = 0;
		note.vel    = 0.0;
		note.down   = false;
		note.amp    = 0.0;
		note.flt    = 0.0;
		note.ampEnv.stage = '_';
		note.fltEnv.stage = '_';
		//note.time = 0; 
		notes[n]    = note;
	}
	
	active = false;
	
	//filtLP = new FilterIIR();
	//filtHP = new butterworth(200.);
	filtLP1 = new BiQuad2(LPF);//, 2000.);
	//filtLP2 = new BiQuad2(LPF);//, 2000.);
	filtHP1 = new BiQuad2(HPF);//, 200.);
	//filtHP2 = new BiQuad2(HPF);//, 200.);
	modvec = false;
	
	delay = new Delay();
	pan = new Pan();
	lfo = new SineTbl();


	lfoNote.num = 1;
	//lfoNote.freq = calcFreq(7.0f);
	lfoNote.phase *= calcFreq(7.0f);
	//lfoNote.phase >>= 0;
	lfoNote.vel = 0.5;
	lfoNote.amp = 1.0; // use this parameter to control assignment


	AddSet( filtLP1, "LP" );
	AddSet( filtHP1, "HP" );
	AddSet( delay, "Dly");
	AddSet( pan,   "Pan" );
	AddParam( &aA, "AmpA" );
	AddParam( &aD, "AmpD" );
	AddParam( &aS, "AmpS" );
	AddParam( &aR, "AmpR" );
	AddParam( &fA, "FltA" );
	AddParam( &fD, "FltD" );
	AddParam( &fS, "FltS" );
	AddParam( &fR, "FltR" );
	AddParam( &dist,    "Dist" );
	AddParam( &amp,     "AmpV" );
	AddParam( &lfo2lp,  "LFLP" );
	AddParam( &env2lp,  "EnLP" );
	AddParam( &lfof,    "LFOF" );
	AddParam( &lfo2amp, "LFOA" );
	AddParam( &loAssign,"LoAs" );
	AddParam( &hiAssign,"HiAs" );
}

void Oscillator::SetParam( int n, fl v )
{
	switch(n)
	{
		case 0x41706D41:  // AmpA
			setADSR( v );
			break;
		case 0x44706D41:  // AmpD
			setADSR( -1, v );
			break;
		case 0x53706D41:  // AmpS
			setADSR( -1, -1, v );
			break;
		case 0x52706D41:  // AmpR
			setADSR( -1, -1, -1, v );
			break;
		case 0x41746C46:  // FltA
			setFilterADSR( v );
			break;
		case 0x44746C46:  // FltD
			setFilterADSR( -1, v );
			break;
		case 0x53746C46:  // FltS
			setFilterADSR( -1, -1, v );
			break;
		case 0x52746C46:  // FltR	
			setFilterADSR( -1, -1, -1, v );
			break;
		case 0x74736944:  // Dist
			setDist( v );
			break;
		case 0x56706D41:  // AmpV
			setAmp( v );
			break;
		case 0x504C6E45:  // EnLP
			setEnv2LP( v );
			break;
		case 0x504C464C:  // LFLP			
			setLFO2LP( v );
			break;
		case 0x464F464C:  // LFOF
			setLFO( -1, v );
			break;
		case 0x414F464C:  // LFOA
			setLFO2Amp( v );
			break;
		case 0x73416948:  // HiAs
			hiAssign = v;
			break;
		case 0x73416F4C:  // LoAs
			loAssign = v;
			break;

	}
}

void Oscillator::setLFO2LP (fl v) { lfo2lp  = v; }
void Oscillator::setLFO2HP (fl v) { lfo2hp  = v; }
void Oscillator::setLFO2Amp(fl v) { lfo2amp = v; }
void Oscillator::setEnv2LP (fl v) { env2lp  = v; }

void Oscillator::setLFO( float a, float f )
{
	//if( a != -1 ) lfoNote.amp = a;
	if( f != -1 )
	{
		lfof = (1 + 19. * f);
		//std::cout << "LFO " << lfof << "\n";
		//lfoNote.freq = calcFreq(f);
		lfoNote.phase *= calcFreq(lfof);
	}
}

void Oscillator::bend(float b)
{
	pitchbend = pow(2, bendInt * b / 12. );
	retemper();
}

void Oscillator::retemper()
{
	for(int n=0; n < 128; n++){
		//if(notes[n].num != 0){
			//notes[n].freq = calcFreq(n);
			notes[n].phase *= calcFreq(n);
		//}
	}
}

void Oscillator::setTemper(Temperament *t)
{
	//if(temper != NULL) delete temper;
	temper = t;
	retemper();
	//calc_chord_ratio();
}

void Oscillator::retune(float A){
	temper->retune(A);
	retemper();
}

void Oscillator::modulate(int m){
	temper->modulate(m);
	retemper();
}

int Oscillator::calcFreq(float f)
{
	static const double dsr = resolution / (double)SAMPLE_RATE;
	return f * pitchbend * dsr;
}

int Oscillator::calcFreq(int n)
{
	return calcFreq((*temper)(n)); //* pitchbend * resolution / (double)SAMPLE_RATE;
}



void Oscillator::setDist(float f)
{
	dist = f;
}

void Oscillator::setDelay(float t, float f)
{
	delay->set(t, f);
}


void Oscillator::setLowpass(fl fc, fl q)
{
	filtLP1->set(fc,q);
	//filtLP2->set(fc,q);
	if( fc != -1) fLP = fc;
	if( q  != -1) qLP = q;
}

void Oscillator::setHighpass(fl fc, fl q)
{
	filtHP1->set(fc,q);
	//filtHP2->set(fc,q);
	if( fc != -1) fHP = fc;
	if( q  != -1) qHP = q;
}


void Oscillator::sustain(int v)
{
	sus = v;

	if( v < 64 ) // initiate release phase for all notes not currently pressed (do this)
	{
		for( int n = 0; n < 128; ++n )
		{
			if(notes[n].ampEnv.stage != '_' and not notes[n].down)
			{
				//notes[n].stage = 'R';
				//notes[n].time = 0;
				noteOff(n);
			}
		}
	}
}

void Oscillator::calc_chord_ratio()
{
	//if( poly <= 1 )
	//	chord_ratio = (*temper)(n);
	/*
	if(poly % 2 == 0)
		chord_ratio /= (*temper)(n);
	else
		chord_ratio /= (*temper)(n);
	*/

	chord_ratio = 1.0;
	float pr = 1.0, cr = 1.0;
	for(int n = lowNote; n <= highNote; n++)
	{
		if(notes[n].down)
		{
			cr = (*temper)(n);
			
			if( pr == 1.0 )
				chord_ratio = cr;
			else
				chord_ratio /= cr;
	
			//std::cout << cr << "/" << pr << "\n";
			pr = cr;
		}
	}
	while( chord_ratio * 2.0 < 1.0 )
		chord_ratio *= 2.0f;
		
	if( chord_ratio > 20 )
		chord_ratio = 1.0;

	chord_ratio = 1.0 / chord_ratio;
	//std::cout <<"chord_ratio = " << chord_ratio << "\n";

}

void Oscillator::noteOn(int n, int v) // needs mode for retriggering in sustain mode
{
	//if(modvec) return;
	modvec = true;

	//notes[n].freq  = calcFreq(n); // this should already be precomputed.
	//notes[n].phase = 0; // let the piano class do this itself.	
	//notes[n].time  = 0;
	curNote = n;
	noteData &note = notes[n];
	note.num   = n;
	note.ampEnv.stage ='A';
	note.fltEnv.stage ='A';
	note.down  = true;
	
	if( v >= 0 )
	{
		note.vel = v / 127.f;
	}	
	
	if(poly < maxPoly - 1) ++poly;

	if(highNote < n) highNote = n;
	if(lowNote  > n) lowNote  = n;
	
	calc_chord_ratio();
	
	modvec = false;
	active = true;

}

void Oscillator::noteOff(int n)
{
	//if(modvec) return;
	modvec = true;
	--poly;
	noteData &note = notes[n];
	note.down = false;
	if( sus < 64 )
	{
		if(note.ampEnv.stage != 'R')
		{
			note.ampEnv.stage = 'R';
		}
		if(note.fltEnv.stage != 'R')
		{
			note.fltEnv.stage = 'R';
		}
	}

	modvec = false;
	if(highNote == n)
		while( highNote >  0  and notes[highNote].num == 0 )
			highNote--;
	if(lowNote  == n)
		while( lowNote  < 127 and notes[lowNote ].num == 0 )
			lowNote++;
	if(lowNote == 0 and highNote == 127){
		/*
		for(int i=0; i < 128; i++){
			notes[i].fltEnv.stage = 'R';
		}
		*/
		active = false;
	}
	for(int c1 = curNote, c2 = curNote; c1 > 0 && c2 < 127; --c1, ++c2){
		if(notes[c1].down){
			curNote = c1;
			break;
		}
		if(notes[c2].down){
			curNote = c2;
			break;
		}
	}
	
	//calc_chord_ratio();

	//if( active )
	//	chord_ratio /= (*temper)(n);
	
	//else
	//chord_ratio = 1.0;
	
	//std::cout << curNote << std::endl;

}

void Oscillator::setADSR(fl a, fl d, fl s, fl r)
{
	if( a != -1 ) aA = a;
	if( d != -1 ) aD = d;
	if( s != -1 ) aS = s;
	if( r != -1 ) aR = r;
	
	for(int i=0; i < 128; ++i)
	{
		notes[i].ampEnv.setADSR(aA,aD,aS,aR);
	}
}

void Oscillator::setFilterADSR(fl a, fl d, fl s, fl r)
{
	if( a != -1 ) fA = a;
	if( d != -1 ) fD = d;
	if( s != -1 ) fS = s;
	if( r != -1 ) fR = r;
	
	for(int i=0; i < 128; ++i)
	{
		notes[i].fltEnv.setADSR(fA,fD,fS,fR);
	}
}


void Oscillator::doADSR(int n)
{
	noteData &note = notes[n];
	note.amp = note.ampEnv.doADSR();
	note.flt = note.fltEnv.doADSR();

	if(note.ampEnv.stage == '_')
	{
		note.fltEnv.stage = '_';
		note.num    = 0;
		note.phase  = 0;
		note.phase2 = 0;
	}
}


int Oscillator::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status)
{
	//#pragma omp critical
	assert(outBuf != NULL);
	float **out = static_cast<float **>(outBuf);

	fl o, sL, sR;

	//int frameClk;
	//if(active){

	for(int i=0; i < framesPerBuf; i++)
	{
		sL = 0.0; //sR = 0.0;

		for(int n = lowNote; n <= highNote; n++)
		{
			noteData &note = notes[n];
			if(note.num != 0)
				doADSR(n);
			if(note.num != 0)
				sL += (*wf)(note) * note.amp * note.vel;
		}
		++clk;
		
		o = applyEffects(sL);
		
		sL = o * pan->ampL;
		sR = o * pan->ampR;
		
		out[0][i] = sL;
		out[1][i] = sR;
	}
	return paContinue;
}


float Oscillator::applyEffects(float o)
{
/*                                                        |
**     LFO      DISTORTION       DELAY        LP    HP    |  AMP
**                                                        |
*/

	fl ao, s, l;
	
	// LFO section
	l = (*lfo)(lfoNote);
	o *= 1.0 - (lfo2amp * (0.5f + l));
	
	// old code:
	//if( lfo2lp > 0.0 )
	//	filtLP->mod(lfo2lp * (0.5f + l));
	
	// new code:
	if( lfo2lp > 0.0 or env2lp > 0.0 )
	{
		filtLP1->mod(
			lfo2lp * (0.5f + l) +
			env2lp * (1 - notes[curNote].flt)
		);
		//filtLP2->mod(
		//	lfo2lp * (0.5f + l) +
		//	env2lp * (1 - notes[curNote].flt)
		//);
	}

	//filtHP->mod(lfo2hp * l);
	
	// distortion section
	ao = fabs(o);
	if( o < 0.0f ) s = -1.0f; else s = 1.0f;
	o = s * powf(ao, dist) * fmaxf(dist, 0.3f); //* fmin(0.8, 1.0 - dist); //* max(0.75f, fabs(dist - 0.5));

	// delay section
	o = delay->process(o);

	///* // filter section
	//if( fLP < 20000.f )
	o = filtLP1->filter(o);
	o = filtHP1->filter(o);
	//o = filtLP2->filter(o);
	//o = filtHP2->filter(o);

	return o * amp;
}

fl Oscillator::queryFilter(fl f)
{
	return 20 * log10f( amp ) + 
		filtLP1->query( f )  +  //filtLP2->query( f )  +
		filtHP1->query( f )  //+  filtHP2->query( f )
	;
}




///////////////////////////////////////////////////////////////////

VariWave::VariWave()
{
	x[0] = 0.;
	y[0] = 0.;
	
	y[1] = 1.;
	
	y[4] = -1.;
	
	x[5] = 1.;
	y[5] = 0.;
}

fl VariWave::func ( fl t )
{	
	int s = 0;
	for(s=1; s < 6; s++)
	{
		if( t < x[s] )
			break;
	}
	return y[s] + ( t - x[s] ) * m[s];
}

void VariWave::shape()
{
	if( wave < 1./3. )
	{
		h = 0; w = 1;
		v = 1. - 3. * wave;
	}
	else if( wave < 2./3. )
	{
		v = 0; w = 1;
		h = 3. * ( wave - 1./3. );
	}
	else
	{
		v = 0; h = 1;
		w = 1. - 3. * ( wave - 2./3. );
	}
	
	x[1] = v/4.;
	
	x[2] = (1+w)/2.;
	y[2] = h;
	
	x[3] = 1. - (1+w)/2.;
	y[3] = -h;

	x[4] = 1. - v/4.;
	
	for(int i=0; i < 5; i++)
	{
		m[i+1] = ( y[i+1] - y[i] ) / ( x[i+1] - x[i] );
	}
}


fl VariWave::operator() (noteData &note)
{
	fl t = note.phase / (fl)SIZE;
	fl y = func( t );
	note.phase++;
	return y;
	
}

/////////////////////////////////////////////////////////////////


WaveTable::WaveTable() : tableSize(SIZE) 
{
	setupTable();
}

fl WaveTable::operator() (noteData &note)
{
	// linear interpolation for fractional wavetable positions
	float y0 = table[note.phase];
	float y1 = table[note.phase+1];
	float f0 = note.phase.frac();
	float f1 = (1.f - f0);
	note.phase++;

/*	// linear interpolation algebra worksheet

	y = y_a + (x - x_a) * (y_b - y_a) / (x_b - x_a)
	f0 = x - x_a
	x_b - x_a ≡ 1
	
	r = y0 + f0 * (y1 - y0)
	r = y0 + (y1 * f0) - (f0 * y0)
	r = y0 - y0 * f0 + y1 * f0
	r = y0 * (1 - f0) + y1 * f0
*/

	return y0 * f1  +  y1 * f0;
//	return table[note.phase++];
}

///////////////////////////////////////////////////////

SineTbl::SineTbl(){
	double f = 2. * M_PI / (double)tableSize;
	for( int t = 0; t < tableSize; t++ ){
		table[t] = (float) sin( t * f );
	}
}

TriangleTbl::TriangleTbl(){
	int t = 0;
	for(; t < tableSize / 4; ++t){
		table[t] = ((float)t) / ((float)tableSize/4.);
	}
	for(; t < 3 * tableSize / 4; ++t){
		table[t] = 2.0 - ((float)t) / ((float)tableSize/4.);
	}
	for(; t < tableSize; ++t){
		table[t] = -4.0 + ((float)t / ((float)tableSize/4.));
	}
}
SquareTbl::SquareTbl(){
	float cycle = 0.5;
	int t = 0;
	int duty = tableSize * cycle;

	table[t] = 0.0f;	
	for(int t=1; t < duty; t++){
		table[t] = 1.0f;
	}
	table[t] = 0.0f;
	for(int t = duty+1; t < tableSize; t++){
		table[t] = -1.0;
	}
}
SawTbl::SawTbl(){
	double f =  2.0 / (double)tableSize;
	int t;
	for(t=0; t < tableSize / 2; t++){
		double tf = t * f;
		table[t] = tf;
	}
	for(; t < tableSize; t++){
		double tf = t * f;
		table[t] = (float)(tf - 2.0);
	}
}

NoiseTbl::NoiseTbl()
{
	int t;
	for(t=0; t < tableSize; t++){
		table[t] = (double)(rand() / (double)RAND_MAX ) - 0.5;
	}
}

/*
void Triangle_analog(){
	double f = M_PI / (double)tableSize;
	for( int t = 0; t < tableSize; t++ ){
		table[t] = 0;
		for(int k = 1; k < 999; k++){
			table[t] += (float) sin( k * M_PI / 2. ) * 
				( (float) sin( 2 * M_PI * k * f * (double)(t) )
					/ (double)( k*k ) );
		}
		table[t] *= 8. / M_PI;
	}
}
*/
/*
void SquareOsc_analog(){
	float f = M_PI  / (double)SAMPLE_RATE;
	for( int t = 0; t < SAMPLE_RATE; t++ ){
		table[t] = 0;
		for(int k = 1; k < 999; k++){
			table[t] += (float) sin( (2 * k - 1) * 2 * f * (double)(t) )
					/ (double)(2.*k - 1);
		}
		table[t] *= 4. / M_PI;
	}
}
*/
/*
void SawOsc_differential(){
	float dx = 0.5 / SAMPLE_RATE;
	float x = 0.;
	for( int t = 0; t < SAMPLE_RATE; t++ ){
		table[t] = x;
		x += dx;
		if( x > 1. ) x = -1.0;
	}
}
*/
/*
void SawOsc::analog(){
	float f = 2. * M_PI / (double)SAMPLE_RATE;
	for( int t = 0; t < SAMPLE_RATE; t++ ){
		table[t] = 0;
		for(int k = 1; k < 999; k++){
			table[t] += (float) sin( f * (double)(k * t) ) / (double)k;
		}
		table[t] *= -2. / M_PI;
	}
}
*/


#include "wx/app.h"
//#ifdef __WXMAC__
#include "wx/filename.h"
//#endif

wxString Oscillator::GetExecutablePath()
{
    static bool found = false;
    static wxString path;

    if (found)
        return path;
    else
    {
        wxString argv0 = wxTheApp->argv[0];

        if (wxIsAbsolutePath(argv0))
            path = argv0;
        else
        {
            wxPathList pathlist;
            pathlist.AddEnvList(wxT("PATH"));
            path = pathlist.FindAbsoluteValidPath(argv0);
        }

        wxFileName filename(path);
        filename.Normalize();
        path = filename.GetFullPath();

        found = true;
        return path;
    }
}

wxString Oscillator::GetSamplePath() // this is a hack
{
	static wxString path = GetExecutablePath();
#ifdef __DARWIN__
	path = path.Left(4 + path.Find(_T(".app")));
	path << _T("/Contents/Resources/Samples/");
#else
	path = _T(".\\Samples\\");
#endif
	return path;
}
