/*
 *  scope.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-07.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#ifndef SCOPE_H
#define	SCOPE_H

//#include <wx/window.h>
#include <wx/wx.h>
#include "main.h"

#define GETDC wxPaintDC pdc(this); /*wxGCDC gdc( pdc ); */ wxDC &dc = (wxDC&) pdc; //PrepareDC(dc); //m_owner->PrepareDC(dc);

class ScopePanel: public wxWindow {
protected:
	int width, height;

public:
	ScopePanel(wxWindow *);
	//~ScopePanel();
	void OnSize(wxSizeEvent&);
	void OnPaint(wxPaintEvent&);
	void OnIdle(wxIdleEvent &);
	void OnTimer(wxTimerEvent &);
	//void append(float,float);
	
	float //minv[SAMPLE_RATE], maxv[SAMPLE_RATE];
		*minv,
		*maxv;
	int pos;
	
	void SetBuffers(float*,float*);

	DECLARE_EVENT_TABLE();

};

#endif
