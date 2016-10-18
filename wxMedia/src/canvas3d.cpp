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
	//EVT_SIZE(Canvas3D::OnSize)
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


static int CanvasAttribs[] = {
	WX_GL_RGBA,
	WX_GL_DOUBLEBUFFER,
	WX_GL_MAJOR_VERSION, 3,
	WX_GL_MINOR_VERSION, 3,
	WX_GL_CORE_PROFILE,
	0
};

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

	teardownProgram(&adsr);
	teardownProgram(&piano);
	teardownProgram(&led);
	teardownProgram(&bcr);
	teardownProgram(&filter);
	teardownProgram(&scope);
}

int Canvas3D::doMessage(const char *s)
{
	int i;
	for(i=0; i < strlen(s); i++)
	{
		str[i] = s[i];
		if( i < 20 )
			led.verts.data[ 7 * i + 6 ] = s[i];
	}
	str[i] = '\0';
	for(; i < 20; i++ )
		led.verts.data[ 7 * i + 6 ] = ' ';
		
	ledAlpha = 1.0; 

	//SetCurrent(*context);
	//glBindBuffer(GL_ARRAY_BUFFER, led.verts.vbo);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, led.verts.size, led.verts.data);

	return 0;
}

void Canvas3D::InitGL()
{
	//SetCurrent();
	//setupVertexBuffers();
	
	ledAlpha = 1.0;
	lissajous_d = 0.0;
	lissajous_rot = 0;
/*
	glClearDepth( 50.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel(GL_FLAT);
	//str = "READY.";
*/

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, 800, 600);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewExperimental = GL_TRUE;
	glewInit();

	setupAdsr();
	setupPiano();
	setupLed();
	setupBcr();
	setupFilter();
	setupScope();

	glBindVertexArray(0);
	
	scope.verts.data = scope_minmaxv;
	//doMessage("!\"#$%&\'()*+,-./0123456789:;<=>?@[\\]^_`{|}~");
}

void Canvas3D::Render()
{
	SetCurrent(*context);
	
	if(not m_init){
		m_init = true;
		InitGL();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	global_time+= 0.01;// = (float)glfwGetTime();
	runProgram(&adsr,true);
	runProgram(&piano,true);
	runProgram(&led,true);
	runProgram(&bcr,true);
	runProgram(&filter);
	runProgram(&scope,true);
	glBindVertexArray(0);
	
	SwapBuffers();
}

void Canvas3D::OnPaint(wxPaintEvent& event)
{
	//if( !IsShown()) return;
	wxPaintDC dc(this);

	//const wxSize ClientSize = GetClientSize();

	//Context3D & canvas = wxGetApp().GetContext(this);
	//glViewport(0, 0, 400, 400);
	//Render();
}


/*
void Canvas3D::OnSize(wxSizeEvent& event)
{
	wxGLCanvas::OnSize(event);
	int w, h;
	GetClientSize(&w,&h);
	//SetCurrent();
	glViewport( 0, 0, (GLint) w, (GLint) h );
	
}
*/

void Canvas3D::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// nop
}

void Canvas3D::OnTimer(wxTimerEvent &)
{
	//Refresh(false);
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
	int x = k-21;
	piano.verts.data[x] = 1;
}

void Canvas3D::keyOff(int k)
{
	int x = k-21;
	piano.verts.data[x] = 0;
}

void Canvas3D::turn(int k, float v)
{
	int col = (k - 1) % 10;
	int row = (k / 10) - 1;
	int x = row * 8 + col;

	if( x < bcr.verts.draw_size )
		bcr.verts.data[x] = v;
}

void Canvas3D::updateFilter(int inst, int i, fl y)
{
	filt[inst][i] = y;
	selinst = inst;

	//filter.verts.data[ i ] = y;
}

void Canvas3D::SetScopeBuffer(float *h)
{
	scope_minmaxv = h;
}

void Canvas3D::hitDrum(int i)
{
	drums[ i % 7 ] = 1.0;
}

void Canvas3D::turnPage(int inst, int pn, float v)
{
	switch( pn )
	{
		case 0x41706D41:  // AmpA
			adsr.verts.data[ 8 * inst + 0 ] = v;
			break;
		case 0x44706D41:  // AmpD
			adsr.verts.data[ 8 * inst + 1 ] = v;
			break;
		case 0x53706D41:  // AmpS
			adsr.verts.data[ 8 * inst + 2 ] = v;
			break;
		case 0x52706D41:  // AmpR
			adsr.verts.data[ 8 * inst + 3 ] = v;
			break;
		case 0x56706D41:  // AmpV
			adsr.verts.data[ 8 * inst + 4 ] = v;
			break;
		default: break;
	}
}
