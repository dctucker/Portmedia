/*
 *  canvas3d.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-04-25.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "canvas3d.h"
#include <wx/wx.h>

BEGIN_EVENT_TABLE(Canvas3D, wxGLCanvas)
	EVT_SIZE(Canvas3D::OnSize)
	EVT_PAINT(Canvas3D::OnPaint)
	//EVT_ERASE_BACKGROUND(Canvas3D::OnEraseBackground)
	//EVT_ENTER_WINDOW( Canvas3D::OnEnterWindow )
	EVT_TIMER( 0, Canvas3D::OnTimer )
END_EVENT_TABLE()



	/*
//holy hell why is this necessary?!
wxVisualAttributes wxWindow::GetDefaultAttributes() const
{
	return GetClassDefaultAttributes(GetWindowVariant());
}
*/

	/*
Canvas3D::Canvas3D( wxFrame *parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size,
	long style, const wxString& name)
	: wxGLCanvas(parent, (wxGLCanvas *) NULL, id, pos, size,
		style | wxFULL_REPAINT_ON_RESIZE, name )
{
	setdefaults();
}

Canvas3D::Canvas3D(wxFrame *parent, const Canvas3D *other,
    wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
    const wxString& name )
    : wxGLCanvas(parent, other->GetContext(), id, pos, size,
	style | wxFULL_REPAINT_ON_RESIZE, name )
{
	setdefaults();
}
*/

	/*
Canvas3D::Canvas3D(wxWindow *parent,
			wxWindowID id,
			int* attribList,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name,
			const wxPalette& palette)
	: wxGLCanvas(parent, id, pos, size,
		style | wxFULL_REPAINT_ON_RESIZE, name )
{
}
*/


static int CanvasAttribs[3] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0};

Canvas3D::Canvas3D( wxWindow *parent ) :
	wxGLCanvas( parent, wxID_ANY, CanvasAttribs, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE )
{
	context = new wxGLContext(this);
	setdefaults();
}

#include "draw_filter.h"
#include "draw_keys.h"
#include "draw_bcr.h"
#include "draw_led.h"
#include "draw_drums.h"
#include "draw_signal.h"
#include "draw_flag.h"
#include "draw_chord.h"

void Canvas3D::setdefaults()
{
    m_init = false;
	scope_width = 0.4;
	mod = 0.0;
	pitch = 0.0;
	vol = 0.75;
	for(int i=0; i < 128; i++) keyon[i] = false;
	rot = 0.0f;
	active[0] = true;
	drums[0] = 1.0;
	drums[1] = 1.0;
	drums[2] = 1.0;
	drums[3] = 1.0;
	drums[4] = 1.0;
	drums[5] = 1.0;
	drums[6] = 1.0;
	bpm = 120.0f;

	curChord.root = 10;
	curChord.type = 1;
	curChord.add2 = 7;
	
	timer = new wxTimer( this, 0 );
}

		
Canvas3D::~Canvas3D()
{
	timer->Stop();
	delete timer;
}

void Canvas3D::InitGL()
{
	//SetCurrent();
	
	glClearDepth( 50.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel(GL_FLAT);
	//str = "READY.";
doMessage("!\"#$%&\'()*+,-./0123456789:;<=>?@[\\]^_`{|}~");
	ledAlpha = 1.0;
	lissajous_d = 0.0;
	lissajous_rot = 0;

}

void Canvas3D::drawOsc()
{
	glPushMatrix();
	
		// draw bitmap
		
		// draw volume ADSR
		glPushMatrix();
		
		glPopMatrix();
		
		// draw LFO settings
		// draw filter ADSR
		// draw filters
		// draw distortion
		// draw delay
		// draw volume
	
	glPopMatrix();
}

void Canvas3D::Render()
{
	SetCurrent(*context);
	
	if(not m_init){
		m_init = true;
		InitGL();
	}

	glEnable(  GL_RESCALE_NORMAL );

	glClearColor( 0.0, 0.0, 0.05, 1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluPerspective( 45.f, 4.0f/3.0f, 0.0f, 20.0f );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	

	//glPushMatrix();
	
	//glRotatef( -45, 1,0,0 );
	//glTranslatef( 0.0, 0.0, -5.0 );

	GLfloat lamb[] = { 1.0f, 1.0f, 1.0f, 0 };
	GLfloat lcol[] = { 1.0f, 0.9f, 0.9f, 1 };
	GLfloat lpos[] = { 0.0f, 0.0f, 2.0f, 0 };
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lamb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lcol );
	glLightfv( GL_LIGHT0, GL_POSITION, lpos );
	
	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 65.0f );
	
	glEnable( GL_LIGHTING );
	glEnable(  GL_LIGHT0  );
	//glEnable(  GL_LIGHT1  );
	glEnable(GL_FOG);


    GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, -0.1f);
    glFogf(GL_FOG_END, 1.0f);
	glFogf(GL_FOG_DENSITY, 0.1f);
	
	
	//glPopMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	drawCube();
	
	//glDisable( GL_LIGHTING );

	//drawLissajous();

	glPushMatrix();

	Enable2D();
		//drawNumber(4);
		drawPiano();
		drawBCR();
		drawScope();
		drawLED();
		drawBPM();
		//drawChord(&curChord);
		
	Disable2D();
	//drawCubanFlag();

	drawFilter();
	drawEnvelope();
	//drawDrums();

	
	glPopMatrix();
	
  	
	glFlush();
	SwapBuffers();
}

void Canvas3D::Enable2D()
{
	//int vPort[4];
	//glGetIntegerv(GL_VIEWPORT, vPort);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glLoadIdentity();

	//glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	//glOrtho( 0.0, 1.0, 1.0, 0.0, 0.0, 1.0 );
	gluPerspective( 45.0f, 4.0f/3.0f, 0.0, 20.0f );

	//glTranslatef( -0.5, -0.5, 0.0 );
	glScalef( 1.0, -1.0, 1.0 );
	glTranslatef( -1.0, -1.0, 0.0 );
	glScalef( 2.0, 2.0, 1.0 );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Canvas3D::Disable2D()
{
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();   
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}

void Canvas3D::OnPaint(wxPaintEvent& event)
{
	//if( !IsShown()) return;
	wxPaintDC dc(this);

	//const wxSize ClientSize = GetClientSize();

	//Context3D & canvas = wxGetApp().GetContext(this);
	//glViewport(0, 0, 400, 400);

}


void Canvas3D::OnSize(wxSizeEvent& event)
{
	wxGLCanvas::OnSize(event);
	int w, h;
	GetClientSize(&w,&h);
	//SetCurrent();
	glViewport( 0, 0, (GLint) w, (GLint) h );
	
}

void Canvas3D::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// nop
}

void Canvas3D::OnTimer(wxTimerEvent &)
{
	Refresh(false);
	//Update();
	Render();
}
