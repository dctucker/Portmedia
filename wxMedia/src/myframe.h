#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/event.h>

class MyFrame : public wxFrame
{
public:
	MyFrame();
protected:

private:
	void OnSize(wxSizeEvent &event);

	DECLARE_EVENT_TABLE()
};
