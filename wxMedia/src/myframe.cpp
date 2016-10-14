#include "myframe.h"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_SIZE(MyFrame::OnSize)
END_EVENT_TABLE()

MyFrame::MyFrame() :
	wxFrame( NULL, wxID_ANY, wxT("Musical canvas"),
		wxPoint(250, 100), wxSize(933,700), 
		( wxDEFAULT_FRAME_STYLE /* | wxFRAME_TOOL_WINDOW */ )
	)
{
}

void MyFrame::OnSize(wxSizeEvent &event)
{
	wxSize size = event.GetSize();
	wxFrame::OnSize(event);
}
