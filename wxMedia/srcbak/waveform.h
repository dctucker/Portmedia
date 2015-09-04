#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "main.h"
#include "wx/wx.h"
#include "fftw3.h"


#define GETDC wxPaintDC pdc(this); /*wxGCDC gdc( pdc ); */ wxDC &dc = (wxDC&) pdc; //PrepareDC(dc); //m_owner->PrepareDC(dc);

class WavePanel: public wxWindow {
	wxWindow *m_owner;
	wxScrollBar *m_scrollbar;
	
	wxBitmap *bmp;
	wxBitmap *spectro_bmp;
	int view_l, view_r, sel_l, sel_r;
	int view_size, max_view, width, height, center_y;
	float **samples;
	int *sel_handle;
	int num_samples;
	int scrollby, zoomby;
	int sel_x1, sel_x2;
	bool painting;
	
	int View2Sample(int);
	int Sample2View(int);
	
	//float *H1, *H2;
	fftwf_plan plan;
	fftwf_complex *buckets;
	bool spectro, spectro_dirty;
public:
	
	WavePanel(wxWindow *);
	~WavePanel();
	void OnSize(wxSizeEvent&);
	void OnScroll(wxScrollEvent&);
	void OnMouse(wxMouseEvent&);
	void OnPaint(wxPaintEvent&);
	void OnIdle(wxIdleEvent &);
	void DrawWaveform(wxDC&);
	void DrawSpectro(wxDC &);
	//float * ComputeDWT(float *, int);
	int logscale(float);
	
	void SetSamples(float **), SetSamples(float *);
	inline float **GetSamples(){ return samples; }
	inline int num(){ return num_samples; }
	
	void ScrollLeft(), ScrollRight();
	
	DECLARE_EVENT_TABLE();
};


#endif
