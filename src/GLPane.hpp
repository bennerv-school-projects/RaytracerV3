#pragma once 

#include <wx/wx.h>
#include <wx/glcanvas.h> 
#include <wx/timer.h>

class BasicGLPane : public wxGLCanvas
{
	wxGLContext*	m_context;

public:
	BasicGLPane(wxFrame* parent, int* args);
	virtual ~BasicGLPane();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& dc);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);


	DECLARE_EVENT_TABLE()
};


class MyFrame : public wxFrame {
public:
	MyFrame();
	~MyFrame() {
		delete timer;
	}
	void UpdateDisplay(wxTimerEvent& evt);
	void OnClose(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	wxTimer *timer;
};