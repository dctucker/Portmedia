/*
 *  canvas3d.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-04-25.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#ifndef CANVAS3D_H
#define CANVAS3D_H

#include "main.h"

#ifdef __WXMAC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <wx/wx.h>
#include <wx/window.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <cmath>           // header for math calculations

#include "midi.h"

#include "chord.h"



class Canvas3D: public wxGLCanvas 
{

public:

	/*
	Canvas3D( wxFrame *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("OpenGL Canvas") );

	Canvas3D( wxFrame *parent, const Canvas3D *other,
		wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0,
		const wxString& name = _T("OpenGL Canvas") );
		*/
	/*
	Canvas3D::Canvas3D(wxWindow *parent,
			wxWindowID id,
			int* attribList = 0,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = 0,
			const wxString& name = wxGLCanvasName,
			const wxPalette& palette = wxNullPalette);
			*/

	Canvas3D(wxWindow *parent );
	virtual ~Canvas3D();
	
	bool m_init;
	
	
	wxTimer *timer;
	wxGLContext *context;
	
	// drum stuff
	int *count;
	int count1;
	float bpm;
	void hitDrum(int);
	float drums[7];
	
	// adsr stuff
	
	// filter stuff
	int selinst;
	float filt[128][128];
	bool active[128];
	void updateFilter(int inst, int i, fl y);
	//void updateEnvelope(int inst, fl a, fl d, fl s, fl r);
	
	fl *ampA[128], *ampD[128], *ampS[128], *ampR[128], *ampV[128];
	
	void setdefaults();
	
	void Render(), InitGL(), Enable2D(), Disable2D(),
		instColor(int,float),
		drawPiano(),
		drawScope(),
		drawCube(),
		drawBCR(),
		drawOsc(),
		drawLissajous(),
		drawFilter(),
		drawEnvelope(),
		drawDrums(),
		drawLED(),
		drawBPM(),
		//drawLEDchar(char,float,float,float,float)
		drawLED( char *str, float r, float g, float b, float a )
		;

	void drawLED16char(char c, float r, float g, float b, float alpha);
	GLuint ledVertexBuffer;
	void setupVertexBuffers();
	
	void OnTimer(wxTimerEvent &);
	void OnPaint(wxPaintEvent&);
	void OnSize(wxSizeEvent &);
	void OnEraseBackground(wxEraseEvent&);

	int doMessage(const char *s){
		int i;
		for(i=0; i < strlen(s); i++){
			str[i] = s[i];
		}
		str[i] = '\0';
		ledAlpha = 1.0; 
		return 0;
	}

	
	float *scope_minv, *scope_maxv, chord_ratio, lissajous_d, lissajous_rot;
	int scope_pos;
	float scope_width;
	chord curChord;
	
	float mod, pitch, vol;
	float rot;

	int loSplit, hiSplit;
	bool keyon[128];

	BCR2000 *bcr;
	
	float ledAlpha;
	char str[256];
	
	void keyOn(int), keyOff(int),
		SetMod(float), SetPitch(float), SetFader(float), SetSustain(float);

	void SetScopeBuffers(float*,float*);

	void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zmin, GLfloat zmax)
	{
		//GLfloat view_height = 600; // perhaps pulled from OS screen resolution
		//GLfloat view_width  = 800; // these would indicate aspect
		GLfloat xmin, xmax, ymin, ymax;
		ymax = zmin * tan(fovy * M_PI / 360.0);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
		glFrustum(xmin, xmax, ymin, ymax, zmin, zmax);
	}

protected:
	DECLARE_EVENT_TABLE();

};

 
//gluPerspective(45.0f, GLfloat(view_width/view_height), 1.0f, 500.0f);


#endif
