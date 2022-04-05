#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "GUI/TicTacGUI.h"
#include "Model/Environment.h"
#define GAMES_PER_REPLAY 512 // games played before a new history replay is done
#define TOTAL_EPISODES_NO 350000 // total number of episodes
#define LR_STEP 50000 // number of episodes before decaying learning rate

class Controller
{
private:
	TicTacFrame* tf; // GUI frame for the game
	Environment env; // environment used to keep track of the board, winning player, etc.
	Agent AI1; // first agent (X player)
	Agent AI2; // second agent (O player)
	short CurTurn; 
	short gameOver; // flag indicating whether current game is over
	short istraining; // flag indicating that training is in progress
public:
	Controller();
	~Controller();
	void TilePressHandler(wxCommandEvent& event);
	void TrainButtonHandler(wxCommandEvent& event);
	void ResetButtonHandler(wxCommandEvent& event);
};

