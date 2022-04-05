///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Nov  5 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bmpbuttn.h>
#include <wx/frame.h>
#include <vector>
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TicTacFrame
///////////////////////////////////////////////////////////////////////////////
class Controller;

class TicTacFrame : public wxFrame
{
private:
	Controller* myCon;
protected:
	wxMenuBar* m_menubar1;
	wxMenu* File;
	wxMenu* AboutMenu;
	wxPanel* Status_Panel;
	wxStaticText* Legend;
	wxStaticText* WinningText;
	wxButton* TrainAIButton;
	wxButton* resetButton;
	wxStaticText* StatsLabel;
	wxPanel* Canvas;
	std::vector <wxBitmapButton*> a_i;
	

	// Virtual event handlers, override them in your derived class
	virtual void ResetClicked(wxCommandEvent& event) { event.Skip(); }
	virtual void Tile(wxCommandEvent& event) { event.Skip(); }


public:

	std::vector<int> buttonIDs;
	TicTacFrame(wxWindow* parent, Controller* con, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~TicTacFrame();
	void displayWinning(int mark);
	void resetWorld();
	int changeButtonImage(int index, int mark);
	void EditStatsText(std::string txt);
};

