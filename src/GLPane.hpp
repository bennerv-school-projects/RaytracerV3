#pragma once 

#include <wx/wx.h>
#include <wx/glcanvas.h> 
#include <wx/timer.h>
#include <pthread.h>


class BasicGLPane : public wxGLCanvas
{
	wxGLContext*	m_context;

public:
	BasicGLPane(wxFrame* parent, int* args, unsigned char * image, int id);
	virtual ~BasicGLPane();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& dc);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);


	DECLARE_EVENT_TABLE()

private:
	unsigned char * image;
	int _id;
};


class MyFrame : public wxFrame {
public:
	MyFrame(int width, int height, int xPos = 50, int yPos = 50);
	~MyFrame() {
		delete timer;
	}
	void UpdateDisplay(wxTimerEvent& evt);
	void OnClose(wxCommandEvent& evt);
	void OnExit(wxCloseEvent& evt);
	void OnSlideEvent(wxCommandEvent& evt);
	DECLARE_EVENT_TABLE();

private:
	wxTimer *timer;
};