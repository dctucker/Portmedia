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
#include <wx/time.h>

BEGIN_EVENT_TABLE(Canvas3D, wxGLCanvas)
	//EVT_SIZE(Canvas3D::OnSize)
	EVT_PAINT(Canvas3D::OnPaint)
	//EVT_ERASE_BACKGROUND(Canvas3D::OnEraseBackground)
	//EVT_ENTER_WINDOW( Canvas3D::OnEnterWindow )
	EVT_TIMER( 0, Canvas3D::OnTimer )
END_EVENT_TABLE()


static int CanvasAttribs[] = {
	WX_GL_RGBA,
	WX_GL_DOUBLEBUFFER,
	WX_GL_MAJOR_VERSION, 3,
	WX_GL_MINOR_VERSION, 3,
	WX_GL_CORE_PROFILE,
	0
};

Canvas3D::Canvas3D( wxWindow *parent ) :
	wxGLCanvas( parent, wxID_ANY, CanvasAttribs, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE ),
	adsr(), fadsr(), blink(), bcr(), filter(), led(), pan(), piano(), scope()
{
	context = new wxGLContext(this);

	setdefaults();
}

void Canvas3D::setdefaults()
{
	//filter.verts.data = &(filt[3][0]);
	fadsr.mvp.yw = 0.0;
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

int Canvas3D::doMessage(const char *s)
{
	int i;
	for(i=0; i < strlen(s); i++)
	{
		str[i] = s[i];
		if( i < 20 )
			led.leds[i].ch = s[i];
	}
	str[i] = '\0';
	for(; i < 20; i++ )
		led.leds[i].ch = ' ';
		
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

	adsr.Setup();
	fadsr.Setup();
	piano.Setup();
	led.Setup();
	pan.Setup();
	bcr.Setup();
	blink.Setup();
	filter.Setup();
	scope.Setup();

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
	ShaderProgram::global_time = (float)( ( wxGetLocalTimeMillis().GetLo() % 1000 ) / 1000.0 );
//std::cout << ShaderProgram::global_time << "\n";

	adsr.Run(true);
	fadsr.Run(true);
	piano.Run(true);
	led.Run(true);
	pan.Run(true);
	bcr.Run(true);
	blink.Run(true);
	filter.Run(true);
	scope.Run(true);

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

	/*
	for( int i=0; i < 8; i++ )
	{
		blink.blinks.amplfo[i].phase = ShaderProgram::global_time;
		blink.blinks.filtlfo[i].phase = ShaderProgram::global_time;
	}
	*/
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

	filter.verts.data[ (inst * 128) + (i % 128) ] = y;
}

/*
void Canvas3D::setBiquad(int inst,float a0, float a1, float a2, float b0, float b1, float b2)
{
	biquad->verts.data[ 9 * inst + 0 ] = a0;	
	biquad->verts.data[ 9 * inst + 1 ] = a1;	
	biquad->verts.data[ 9 * inst + 2 ] = a2;	
	biquad->verts.data[ 9 * inst + 3 ] = b0;	
	biquad->verts.data[ 9 * inst + 4 ] = b1;	
	biquad->verts.data[ 9 * inst + 5 ] = b2;	
}
*/

void Canvas3D::SetScopeBuffer(float *h)
{
	scope_minmaxv = h;
}

void Canvas3D::hitDrum(int i)
{
	drums[ i % 7 ] = 1.0;
}

void Canvas3D::LoadSet(Set *set, int inst)
{
	for(int s=0; s < set->scount; ++s)
	{
		LoadSet(set->set[s], inst);
	}
	for(int p=0; p < set->pcount; p++)
	{
		SetParam(inst, set->param[p]->name.v, set->param[p]->GetValue());
	}
}

void Canvas3D::SetParam(int inst, int pn, float v)
{
	switch( pn )
	{
		// Amp ADSR
		case P_AmpV: adsr.adsrs[inst].v = v; break;
		case P_AmpA: adsr.adsrs[inst].a = v; break;
		case P_AmpD: adsr.adsrs[inst].d = v; break;
		case P_AmpS: adsr.adsrs[inst].s = v; break;
		case P_AmpR: adsr.adsrs[inst].r = v; break;

		// Filt ADSR
		case P_EnLP: fadsr.adsrs[inst].v = v; break;
		case P_FltA: fadsr.adsrs[inst].a = v; break;
		case P_FltD: fadsr.adsrs[inst].d = v; break;
		case P_FltS: fadsr.adsrs[inst].s = v; break;
		case P_FltR: fadsr.adsrs[inst].r = v; break;

		case P_LFOF: // LFO Frequency
			blink.blinks.amplfo[inst].freq = v;
			blink.blinks.filtlfo[inst].freq = v;
			blink.blinks.amplfo[inst].phase = ShaderProgram::global_time;
			blink.blinks.filtlfo[inst].phase = ShaderProgram::global_time;
			break;
		case P_Time: // Delay Time
			blink.blinks.delay[inst].freq = v;
			break;

		case P_LFOA: // LFO to Amp
			blink.blinks.amplfo[inst].amp = v;
			break;
		case P_LFLP: // LFO to Lowpass
			blink.blinks.filtlfo[inst].amp = v;
			break;
		case P_Fdbk: // Delay Feedback
			blink.blinks.delay[inst].amp = v;
			break;

		case P_PanB: // Pan Balance
			pan.pans[inst].pan = v;
			break;

		case P_LoAs: // Low Assign
			// assign.data[inst].lo_on = v;
			break;
		case P_HiAs: // High Assign
			// assign.data[inst].hi_on = v;
			break;
		case P_LoSp: // Lower Split
			// assign.data[inst].lo_split = v;
			break;
		case P_HiSp: // High Split
			// assign.data[inst].hi_split = v;
			break;
		case P_Shlf: // Lower Shelf
			// assign.data[inst].lo_shelf = v;
			break;

		case P_Dist: // Distortion
			// shape.data[inst].distortion = v;
			break;
		case P_Detu: // Detune
			// shape.data[inst].detune = v;
			break;

		case P_Mix:  // Piano Octave Mix
			break;
		case P_Freq: // Filter Frequency
		case P_Reso: // Filter Resonance
		case P_Gain: // Filter Gain
			break;
		default: break;
	}
}
