/*
 *  hammond.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-21.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "hammond.h"

#include <iostream>

Hammond::Hammond()
{
	wt = new WaveTable();
	//tableSize = 96000;
	/*
	double f = 2. * M_PI / (double)tableSize;
	for( int t = 0; t < tableSize; t++ ){
		table[t] = (float) sin( t * f );
	}*/

	
	wxString path = GetSamplePath();
	
	path.Append(_T("Hammond_.wav"));
	
	//std::cout << std::endl << path.mb_str() << std::endl;
	SndfileHandle *sf = new SndfileHandle(path.mb_str(), SFM_READ);

	if( not sf->error() )
	{
		float ham[96000];
		sf->read(ham, 96000 );
		
		// it's in memory, now to resample
		SRC_DATA sd;
		sd.data_in       = ham;
		sd.data_out      = wt->table;
		sd.input_frames  = 96000;
		sd.output_frames = wt->tableSize;
		sd.src_ratio = ((double)wt->tableSize)/96000.;
		src_simple(&sd, SRC_SINC_BEST_QUALITY, 1);

	} else {
		exit(2);
	}
	
	delete sf;

}

// a good Hammond has harmonic leakage ± 4 octaves on each note

// nine-stage delay line for chorus / vibrato
// delay by LC phase shift circuits... short delay of 1 ms
// 16-step scanning: 1 to 9, and back 9 to 1 (delay time = triangle wave)
// rate is constant 7 Hz
