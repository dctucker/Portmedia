/*
 *  adsrui.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-17.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#ifndef ADSRUI_H
#define	ADSRUI_H

#include <wx/wx.h>
#include "main.h"

#define GETDC wxPaintDC pdc(this); /*wxGCDC gdc( pdc ); */ wxDC &dc = (wxDC&) pdc; //PrepareDC(dc); //m_owner->PrepareDC(dc);

class ADSRPanel: public wxWindow {
protected:
	int width, height;

public:
	ADSRPanel(wxWindow *);
	//~ScopePanel();
	void OnSize(wxSizeEvent&);
	void OnPaint(wxPaintEvent&);
	void OnIdle(wxIdleEvent &);

	void set(float,float,float,float);
	void append(float,float);
	
	float a,d,s,r;
	int pos;

	DECLARE_EVENT_TABLE();

};

class FilterPanel: public wxWindow {
protected:
	int width, height;

public:
	FilterPanel(wxWindow *);
	//~ScopePanel();
	void OnSize(wxSizeEvent&);
	void OnPaint(wxPaintEvent&);
	void OnIdle(wxIdleEvent &);

	void set(float,float,float,float);
	void append(float,float);
	
	float f1,q1,f2,q2;

	DECLARE_EVENT_TABLE();

};

#endif
