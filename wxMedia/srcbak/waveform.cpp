/*
 *  waveform.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2009-11-13.
 *  Copyright 2009 DCT. All rights reserved.
 *
 */

#include "waveform.h"
#include <iostream>

#define PI 3.141592653589793
#define WINDOW 4800
#define ANGULAR( f ) ((f) * 2.0 * PI / (float)SAMPLE_RATE)

BEGIN_EVENT_TABLE(WavePanel, wxWindow)
    EVT_PAINT( WavePanel::OnPaint )
	EVT_MOUSE_EVENTS( WavePanel::OnMouse )
	EVT_SCROLL( WavePanel::OnScroll )
	EVT_SIZE( WavePanel::OnSize )
	EVT_IDLE( WavePanel::OnIdle )
//	EVT_TIMER( WavePanel::OnTimer )
END_EVENT_TABLE()

WavePanel::WavePanel(wxWindow *parent) : 
	wxWindow(parent, wxID_ANY)
{
	spectro = false;
	m_owner = parent;
	painting = false;
	num_samples = 96000 * 30;
	samples = new float*[2];
	samples[0] = new float[num_samples];
	samples[1] = new float[num_samples];
	scrollby = 96; zoomby = 960;
	view_l = 0; view_r = 96000;
	sel_l = 4000; sel_r = 5000;
	sel_handle = &sel_l;
	view_size = view_r - view_l;
	float w_1 = ANGULAR( 200.0);
	float w_2 = ANGULAR(2000.0);
	float samp;

	for(int t = 0; t < num_samples; t++){
		samp  = 0.49 * sin(t * w_1);
		samp += 0.49 * sin(t * w_2);
		samples[0][t] = samp;
	}

	width = 400; height = 200;
	m_scrollbar = new wxScrollBar(this, wxID_ANY, wxPoint(0, height - 32), wxSize( width - 16, 16) );
	m_scrollbar->SetScrollbar(0, view_size, num_samples, 1 + view_size / 2);

	buckets = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * WINDOW);
	plan = fftwf_plan_dft_r2c_1d(WINDOW, samples[0], buckets, FFTW_ESTIMATE);
	fftwf_execute(plan);
	/*
	for(int i=0; i < WINDOW; i++){
		std::cout << (float)(fabs(buckets[i][0]) + fabs(buckets[i][1])) << std::endl;
	}
	*/
	spectro_bmp = new wxBitmap(width, height);
}

WavePanel::~WavePanel(){
	delete spectro_bmp;
	delete m_scrollbar;
	//delete H1, H2;
	delete samples[0], samples[1], samples;
	fftwf_destroy_plan(plan);
	fftwf_free(buckets);
}


void WavePanel::OnIdle(wxIdleEvent & WXUNUSED(event)){
	Refresh();
}

void WavePanel::SetSamples(float **buf){
	samples = buf;
}

void WavePanel::SetSamples(float *buf){
	samples[0] = buf;
}

void WavePanel::OnSize(wxSizeEvent& e){
	wxSize s = e.GetSize();
	height = s.GetHeight();
	width = s.GetWidth();
	m_scrollbar->SetSize( 0, height - 16, width - 16, 16 );
}

void WavePanel::OnScroll(wxScrollEvent& e){
	int k = e.GetPosition();
	view_l = k;
	view_r = k + view_size;
	Refresh();	
}

void WavePanel::ScrollLeft(){
	if(view_l - scrollby > 0){
		view_l -= scrollby;
		view_r -= scrollby;
	}
	Refresh();
	Update();
}

void WavePanel::ScrollRight(){
	if(view_r + scrollby < num_samples){
		view_l += scrollby;
		view_r += scrollby;
	}
	Refresh();
	//Update();
}

// return a sample given a coordinate
int WavePanel::View2Sample(int x){ 
	return (x / (float)width) * view_size + view_l; 
}

// return a coordinate given a sample (unused)
int WavePanel::Sample2View(int i){ // need to fix selection to work with scrolled offset
	return ((i - view_l) / (float)view_size) * width;
}

void WavePanel::OnMouse(wxMouseEvent &e){
	int x = e.GetX();
	int samp_x = View2Sample(x);
	
	if(e.LeftDClick()){
		//sel_l = View2Sample(x);
		//sel_r = sel_l;
		spectro = !spectro;
		spectro_dirty = spectro;
		
	} else if(e.LeftDown()){
		// this selects the closest selector as a handle
		sel_handle = abs(samp_x - sel_l) < abs(samp_x - sel_r) ? &sel_l : &sel_r;
		
	} else if(e.LeftIsDown()){
		*sel_handle = View2Sample(x);
		Refresh();
		
	} else if(e.LeftUp()){
		*sel_handle = View2Sample(x);
		if( sel_r < sel_l ){
			int sel_s = sel_l;
			sel_l = sel_r;
			sel_r = sel_s;
		}
		Refresh();
		
	} else if(e.GetWheelRotation()){ // use wheel for zoom
		/*
		if( view_size < 9600 ) zoomby = 96;
		else if( view_size < 96000 ) zoomby = 960;
		else zoomby = 9600;
		*/
		zoomby = 1 + view_size / 10;
		
		int w = e.GetWheelRotation();
		float cw = x / (float)width;
		int old_l = view_l, old_r = view_r;

		if( w > 0 ){ // zoom in
			view_l += zoomby * cw;
			view_r -= zoomby * (1 - cw);
			if( view_r - zoomby < 0) view_r = 0;

			if( view_r < view_l ) {
				view_l = old_l;
				view_r = old_r;
			}

		} else { // zoom out
			view_l -= zoomby * cw;
			view_r += zoomby * (1 - cw);
			if( view_l - zoomby < 0)  view_l = 0;
		}
		m_scrollbar->SetScrollbar( view_l, view_size, num_samples, 1 + view_size / 2);
		Refresh();
	}
}

void WavePanel::OnPaint(wxPaintEvent &WXUNUSED(event)){
	//painting = false;
	//if(!painting){
		painting = true;
		GETDC;
		
		wxSize s = GetClientSize();
		width = s.GetWidth();
		wxSize scroll_s = m_scrollbar->GetClientSize();
		height = s.GetHeight() - 1 - scroll_s.GetHeight();

		center_y = height / 2;
		view_size = view_r - view_l;
		max_view = view_r >= num_samples - 1 ? num_samples - view_l - 1 : view_size;


		if(spectro) {
			dc.SetBackground(wxBrush(wxColor(0x00,0x00,0x00)));
			dc.Clear();

			if(spectro_dirty) {
				wxMemoryDC sdc;
				if( spectro_bmp->GetWidth() < width or spectro_bmp->GetHeight() < height ){
					delete spectro_bmp;
					spectro_bmp = new wxBitmap(width, height);
				}
				sdc.SelectObject(*spectro_bmp);
				sdc.SetBrush(wxColor(0x00,0x00,0x00,0xFF));
				sdc.Clear();
				DrawSpectro(sdc);
				spectro_dirty = false;
			}
			dc.DrawBitmap(*spectro_bmp, 0, 0, true);
			
		// clear display
		} else {
			dc.SetBackground(wxBrush(wxColor(0x00,0x22,0x11)));
			dc.Clear();
		}
			DrawWaveform(dc);

		// draw selection
		dc.SetBrush(wxBrush(wxColor(0x99, 0x99, 0x55, 0x80)));
		dc.SetPen(wxPen(wxColor(0x00,0x00,0x00,0x00)));
		sel_x1 = ((sel_l - view_l) / (float)view_size) * width;
		sel_x2 = ((sel_r - view_l) / (float)view_size) * width;
		dc.DrawRectangle( sel_x1, -1, sel_x2 - sel_x1, height + 3 );


		int w, h; 
		dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTWEIGHT_NORMAL, 0));
		dc.SetTextForeground(wxColor(0x33,0x66,0x33));

		// indicate window size		
		wxString s1; s1 << view_l;
		wxString s2; s2 << view_r;
		dc.DrawText(s1, 0, 0);
		dc.GetTextExtent(s2, &w, &h);
		dc.DrawText(s2, width - w, 0);

		// indicate selection area
		wxString s3, s4;
		s3 << sel_l; s4 << sel_r;
		dc.GetTextExtent( s3, &w, &h );
		dc.DrawText(s3, sel_x1 - w, height - h);
		//dc.GetTextExtent( s4, &w, &h );
		dc.DrawText(s4, sel_x2, height - h);

		painting = false;
	//}
}

int WavePanel::logscale(float y){
	// n ∈ 1.0 .. 4.0 , f ∈ 20 .. 20,000 , g ∈ 0.0 .. 1.0 , r ∈ 0 .. WIN
	
	float //MIN_FREQ = 20.0
		MAX_FREQ = 20000.0;
	
	float n = 3.0 * y / (float)height  +  1.0;
	float f = 2 * pow(10, n);
	//float g = (f - MIN_FREQ) / (MAX_FREQ - MIN_FREQ);
	float g = f / MAX_FREQ;
	
	return floor(WINDOW * g * ( MAX_FREQ / (float)SAMPLE_RATE ) );
}

void WavePanel::DrawSpectro(wxDC& dc){
	//float stride_y = floor(WINDOW / height) / 2;
	int pixels[height];
	float len_x = 1.0;
	int rgb[256][4];
	
	for(int i = 0; i < 256; i++ ){
		int r,g,b,a;
		int h,v;
		v = i / 64;
		h = 4 * (i % 64);
		r = (h < 1)   ? 0 :
				(h < 96)  ? (63 + h * 2) : 
				(h < 192) ? (255 - ((h - 95) * 4))
						  : 0;
		
		g = (h < 64)  ? 0 : 
				(h < 192) ? (h - 64) * 2
						  : (255 - (h - 192));
		
		b = (h < 128) ? 0
						  : (h - 128) * 2;
						  
		a = 128 + 32 * v;
		

		rgb[i][0] = r / (4 - v);
		rgb[i][1] = g / (4 - v);
		rgb[i][2] = b / (4 - v);
		rgb[i][3] = 244; //a / (4 - v);
		
	}
	
	int freqs[height];
	
	for( int x = 0; x < width / len_x; x ++) {
		fftwf_execute_dft_r2c(plan, &(samples[0][x * WINDOW/8]), buckets);
		//float d = sqrt(buckets[0][0]);
		if( x < 50 ) std::cout << "DC = " << buckets[0][0] << std::endl;
		int g1 = logscale(0); if(g1 < 1) g1 = 1;
		int v = 0;
		float v_max = 1;
		
		
		// compute a segment of the spectrogram
		for(int y = 0; y < height; y++) {
			
			//int g1 = logscale(y); if( g1 == 0 ) g1 = 1;
			int g2 = logscale(y+1);
			freqs[y] = SAMPLE_RATE * (g2 /(float) WINDOW);
			if(g1 < g2){
				v = 0;
				for(int j = g1; j < g2; j++) {
					//v += abs( buckets[j][0] ) + abs( buckets[j][1] );
					v += sqrt(  buckets[j][0] * buckets[j][0] + 
					            buckets[j][1] * buckets[j][1]   );
				}
				//v /= d;
				
				//v *= 510;
				//v *= buckets[0][0];
				//v /= (float)WINDOW;
				if( v > v_max ) v_max = v;
				
				//std::cout 
			}
			
			pixels[y] = v;
			g1 = g2;
		}
		
		// and here we draw the computed spectrogram using the 256 color pallette
		
		int lastFreq = 0;
		for(int y = 0; y < height; y++){
			v = pixels[y]; //255 * pixels[y] / v_max;
			//if( x < 50 and v > 0 ) std::cout << v << std::endl;
			
			if( v > 255 ) v = 255;

			int r = rgb[v][0];
			int g = rgb[v][1];
			int b = rgb[v][2];
			int a = rgb[v][3];
			dc.SetPen(wxColor(r,g,b,a));
			dc.DrawLine(x * len_x, height - y, (x+1) * len_x - 1, height - y);
			
			//g1 = g2;
			
			if( y % 20 == 1 and lastFreq != freqs[height - y]) {
				wxString freqStr;
				freqStr << freqs[height - y];
				dc.SetTextForeground(wxColor(192,192,192));
				dc.SetFont(wxFont(8,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
				dc.DrawText(freqStr, width / 2, y);
				lastFreq = freqs[height - y];
			}

		}
		
	}

/*
	float stride_x = 8192;  //view_size / width;
	float stride_y = stride_x / height;
	for( int x = 0; x < width; x++ ){
		float *H = ComputeDWT( &(samples[0][(int)(x * stride_x)]) , (int)stride_x);
		float max_h = H[0];
		for( int y = 0; y < height; y++ ){
			int v = 255 * H[(int)(y * stride_y)] / max_h;
			int r = v < 64 ? 4 * v : 0;
			int g = (v > 64 and v < 192) ? 2 * (v - 64) : 0;
			int b = v > 192 ? (v - 192) * 3 : 0;
			dc.SetPen(wxColor(r,g,b));
			dc.DrawPoint(x, y);
		}
		//delete H;
	}
*/
}

/*
float* WavePanel::ComputeDWT(float* input, int input_length) {
 
    //This function assumes input.length=2^n, n>1
	for( int i = 0; i < input_length; i++){
		H1[i] = input[i];
	}
 
    for (int length = input_length >> 1; ; length >>= 1) {

        //length=2^n, WITH DECREASING n
        for (int i = 0; i < length; i++) {
            float sum = H1[ 2 * i ] + H1[ 2 * i  +  1];
            float dif = H1[ 2 * i ] - H1[ 2 * i  +  1];
            H2[ i ] = sum;
            H2[ i + length ] = dif;
        }
        if (length == 1) {
			//delete H1;
			return H2;
		}

        //Swap arrays to do next iteration
        //System.arraycopy(output, 0, input, 0, length<<1);
		float *temp = H1; H1 = H2; H2 = temp;
    }
}
*/

void WavePanel::DrawWaveform(wxDC& dc){
	// draw centroid
	dc.SetPen(wxPen(wxColor(0x00, 0x11, 0x00)));
	dc.DrawLine(0, center_y, width, center_y);

	// draw waveform
	int stride = view_size / width;
	if( stride < 1) {
		stride = 1;
		view_r = view_l + width;
		view_size = width;
	}
	float v;
	for( int x = 0; x < width; x++){
		
		float min_v = 2.0;
		float max_v = -2.0;
		
		if( view_l + (x+1) * stride > num_samples) break;
		
		for( int i = 0; i <= stride; i++){
			v = samples[0][view_l + (int)(x * stride) + i];
			if( v > max_v) max_v = v;
			if( v < min_v) min_v = v;
		}
		int y1 = center_y - min_v * height / 2;
		int y2 = center_y - max_v * height / 2;
		
		dc.SetPen(wxPen(wxColor(0x00, 0xFF, 0x00)));
		dc.DrawLine(x, y1, x, y2 );
		
		// enhancing the line at edges?
		//dc.SetPen(wxPen(wxColor(0x00, 0x88, 0x00)));
		//dc.DrawPoint(x, y1 - 1);
		//dc.DrawPoint(x, y2 + 1);
	}
	/* 
	// draw by samples
	for( int i = 0; i < max_view; i++ ){
		x = width * (i / (float)view_size);
		v = -samples[i + view_l] * height / 2;
		y = center_y + v;
		dc.DrawPoint(x, y); //, x+1, y);
	}
	*/

}