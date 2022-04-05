///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Nov  5 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "TicTacGUI.h"
#include "Controller.h"

///////////////////////////////////////////////////////////////////////////

TicTacFrame::TicTacFrame(wxWindow* parent, Controller* con, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	myCon = con;
	a_i = std::vector <wxBitmapButton*>(9, NULL);
	this->SetSizeHints(wxSize(-1, -1), wxSize(-1, -1));

	m_menubar1 = new wxMenuBar(0);
	File = new wxMenu();
	wxMenuItem* New_MenuItem;
	New_MenuItem = new wxMenuItem(File, wxID_ANY, wxString(wxT("New")), wxEmptyString, wxITEM_NORMAL);
	File->Append(New_MenuItem);

	wxMenuItem* Stats_MenuItem;
	Stats_MenuItem = new wxMenuItem(File, wxID_ANY, wxString(wxT("Stats")), wxEmptyString, wxITEM_NORMAL);
	File->Append(Stats_MenuItem);

	wxMenuItem* Exit_MenuItem;
	Exit_MenuItem = new wxMenuItem(File, wxID_ANY, wxString(wxT("Exit")), wxEmptyString, wxITEM_NORMAL);
	File->Append(Exit_MenuItem);

	m_menubar1->Append(File, wxT("File"));

	AboutMenu = new wxMenu();
	wxMenuItem* About_MenuItem;
	About_MenuItem = new wxMenuItem(AboutMenu, wxID_ANY, wxString(wxT("About")), wxEmptyString, wxITEM_NORMAL);
	AboutMenu->Append(About_MenuItem);

	m_menubar1->Append(AboutMenu, wxT("About"));

	this->SetMenuBar(m_menubar1);

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer(wxHORIZONTAL);

	Status_Panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	Status_Panel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVEBORDER));
	Status_Panel->SetBackgroundColour(wxColour(201, 201, 201));
	Status_Panel->SetMinSize(wxSize(200, -1));
	Status_Panel->SetMaxSize(wxSize(200, -1));

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	Legend = new wxStaticText(Status_Panel, wxID_ANY, wxT("Player 1: X\nPlayer 2: O"), wxDefaultPosition, wxDefaultSize, 0);
	Legend->Wrap(-1);
	Legend->SetFont(wxFont(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));
	Legend->SetForegroundColour(wxColour(5, 23, 186));
	Legend->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));

	bSizer2->Add(Legend, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	WinningText = new wxStaticText(Status_Panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	WinningText->Wrap(-1);
	WinningText->SetFont(wxFont(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New")));
	WinningText->SetForegroundColour(wxColour(136, 0, 68));
	WinningText->SetBackgroundColour(wxColour(201, 201, 201));

	bSizer2->Add(WinningText, 0, wxALIGN_LEFT| wxALIGN_CENTER_VERTICAL  | wxALL, 15);


	bSizer2->Add(0, 0, 1, wxEXPAND, 5);

	TrainAIButton = new wxButton(Status_Panel, wxID_ANY, wxT("Train AI"), wxDefaultPosition, wxDefaultSize, 0);
	TrainAIButton->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	bSizer2->Add(TrainAIButton, 0, wxALIGN_CENTER | wxALL, 5);

	resetButton = new wxButton(Status_Panel, wxID_ANY, wxT("Reset Board"), wxDefaultPosition, wxDefaultSize, 0);
	resetButton->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	bSizer2->Add(resetButton, 0,  wxALIGN_CENTER | wxALL, 5);

	StatsLabel = new wxStaticText(Status_Panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	StatsLabel->Wrap(-1);
	StatsLabel->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial")));
	StatsLabel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));

	bSizer2->Add(StatsLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	bSizer2->Add(0, 0, 4, wxEXPAND, 5);

	Status_Panel->SetSizer(bSizer2);
	Status_Panel->Layout();
	bSizer2->Fit(Status_Panel);
	mainSizer->Add(Status_Panel, 1, wxEXPAND | wxALL, 0);

	Canvas = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxTAB_TRAVERSAL);
	Canvas->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
	Canvas->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	Canvas->SetMinSize(wxSize(600, 600));
	Canvas->SetMaxSize(wxSize(600, 600));

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer(3, 3, 0, 0);
	int i;
	for(i=0;i<9;i++)
		buttonIDs.push_back(NewControlId());

	for (i = 8; i >= 0; i--)
	{// build a button
		a_i[i] = new wxBitmapButton(Canvas, buttonIDs[i], wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | 0);
		a_i[i]->SetBitmap(wxBitmap(wxT("Images/Blank_.bmp"), wxBITMAP_TYPE_ANY));
		gSizer1->Add(a_i[i], 0, wxALL, 1);
		a_i[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Controller::TilePressHandler, myCon);
	}

	Canvas->SetSizer(gSizer1);
	Canvas->Layout();
	gSizer1->Fit(Canvas);
	mainSizer->Add(Canvas, 3, wxEXPAND | wxALL, 0);


	this->SetSizer(mainSizer);
	this->Layout();
	mainSizer->Fit(this);

	this->Centre(wxBOTH);

	// Connect Events
	resetButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Controller::ResetButtonHandler, myCon);
	TrainAIButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Controller::TrainButtonHandler, myCon);
}

TicTacFrame::~TicTacFrame()
{
	// Disconnect Events
	resetButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &Controller::ResetButtonHandler, myCon);
	TrainAIButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &Controller::TrainButtonHandler, myCon);

	int i;
	for (i = 8; i <= 0; i--)
	{// build a button
		a_i[i]->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &Controller::TilePressHandler, myCon);
	}
}

int TicTacFrame::changeButtonImage(int index, int mark)
{
	if (index > 8 || index < 0)
		return -1;

	if (mark == XMARK) // X
		a_i[index]->SetBitmap(wxBitmap(wxT("Images/X_.bmp"), wxBITMAP_TYPE_ANY));
	else if (mark == OMARK)
		a_i[index]->SetBitmap(wxBitmap(wxT("Images/O_.bmp"), wxBITMAP_TYPE_ANY));
	return 0;
}

void TicTacFrame::displayWinning(int mark)
{
	if (mark == XMARK)
		WinningText->SetLabel("Player 1 wins");
	else
		WinningText->SetLabel("Player 2 wins");
	if(mark == 0)
		WinningText->SetLabel("");
}

void TicTacFrame::resetWorld()
{
	int i;
	for (i = 0; i < a_i.size(); i++)
	{
		a_i[i]->SetBitmap(wxBitmap(wxT("Images/Blank_.bmp"), wxBITMAP_TYPE_ANY));
	}
	displayWinning(0);
}

void TicTacFrame::EditStatsText(std::string txt)
{
	StatsLabel->SetLabelText(txt);
}