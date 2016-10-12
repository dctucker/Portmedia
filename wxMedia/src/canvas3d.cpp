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

#include "shaders/common.h"
#include "shaders/filter.h"
#include "shaders/piano.h"
#include "shaders/bcr.h"
#include "shaders/led.h"
//#include "shaders/drums.h"
#include "shaders/scope.h"
#include "shaders/adsr.h"
//#include "shaders/flag.h"
//#include "shaders/chord.h"

inline static void drawLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);
	global_time++;// = (float)glfwGetTime();
	runProgram(&adsr);
	runProgram(&piano);
	runProgram(&led);
	runProgram(&bcr);
	runProgram(&filter);
	runProgram(&scope);
	glBindVertexArray(0);
}

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
	//setupVertexBuffers();
	
	doMessage("!\"#$%&\'()*+,-./0123456789:;<=>?@[\\]^_`{|}~");
	ledAlpha = 1.0;
	lissajous_d = 0.0;
	lissajous_rot = 0;
/*
	glClearDepth( 50.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel(GL_FLAT);
	//str = "READY.";

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, 800, 600);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/

	glewExperimental = GL_TRUE;
	glewInit();

	setupAdsr();
	setupPiano();
	setupLed();
	setupBcr();
	setupFilter();
	setupScope();

	glBindVertexArray(0);
}

void Canvas3D::Render()
{
	SetCurrent(*context);
	
	if(not m_init){
		m_init = true;
		InitGL();
	}

/*
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


	//drawCube();
	
	//glDisable( GL_LIGHTING );

	//drawLissajous();

    drawLoop();
  	
*/
	glFlush();
	SwapBuffers();
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
void Canvas3D::SetMod(float f)
{
	mod = f;
}
void Canvas3D::SetSustain(float f)
{
	// nop
}

void Canvas3D::SetPitch(float f)
{
	pitch = f;
}
void Canvas3D::SetFader(float f)
{
	vol = f;
}

void Canvas3D::keyOn(int k)
{
	keyon[k] = true;
}

void Canvas3D::keyOff(int k)
{
}

void Canvas3D::updateFilter(int inst, int i, fl y)
{
	filt[inst][i] = y;
	selinst = inst;
}

void Canvas3D::SetScopeBuffers(float *h, float *l)
{
	scope_maxv = h;
	scope_minv = l;
}

void Canvas3D::hitDrum(int i)
{
	drums[ i % 7 ] = 1.0;
}

