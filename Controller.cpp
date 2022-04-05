#include "Controller.h"


Controller::Controller()
{
	CurTurn = 1;
	gameOver = 0;
	istraining = 0;
	AI1.setMark(XMARK);
	AI2.setMark(OMARK);
	AI1.loadValues("X_Training.csv");
	AI2.loadValues("O_Training.csv");
	tf = new TicTacFrame(NULL,this,-1,"TicTacToe Bot");
	env.resetBoard();
	tf->Show();
}
Controller::~Controller()
{
	delete tf;
}

void Controller::TrainButtonHandler(wxCommandEvent& event)// without agent being inside the environment
{

	int i, index;
	int rate_counter=0;
	int moveResult;
	int actionX,actionO;
	int curState;
	istraining = 1;
	int gameCounter = 0;
	AI1.resetState();
	AI2.resetState();
	env.resetBoard();
	AI1.loadValues("X_Training.csv");
	AI2.loadValues("O_Training.csv");
	AI1.setEps(0.1);
	AI2.setEps(0.1);

	
	tf->EditStatsText("Training...");

	//train for TOTAL_EPISODES_NO episodes
	for(long long j=0;j< TOTAL_EPISODES_NO;) 
	{
		
		if (gameOver|| env.isFull())
		{
		// reset board on end of episode
			AI1.resetState();
			AI2.resetState();
			env.resetBoard();
			gameOver = 0;
			j++;
			rate_counter++;
			gameCounter++;
			if (gameCounter > GAMES_PER_REPLAY) // replay experience every (GAMES_PER_REPLAY) games
			{
				AI1.learn_replay();
				AI2.learn_replay();
				gameCounter = 0;
			}
			if (rate_counter > LR_STEP)
			{ // reduce learning rate as training progresses
				AI1.setLearningRate(AI1.getLearningRate() / 2);
				AI2.setLearningRate(AI2.getLearningRate() / 2);
				rate_counter = 0;
			}
		}

		if (!gameOver)
		{
			
			//---------------------------- x move --------------------------
			actionX = AI1.playNextMove();
			moveResult=env.Board_Move(actionX, XMARK);
			curState = env.getState();
			if (moveResult == XMARK) // X wins
			{
				AI1.add_MC_step(curState, 1);
				AI2.add_MC_step(curState,-1);
				AI1.apply_MC();
				AI2.apply_MC();
				gameOver = 1;
			}
			else if (moveResult == 0) // Valid move but no one won
			{
				AI1.add_MC_step(curState, 0);
				AI2.add_MC_step(curState, 0);
				if (env.isFull())
				{
					AI1.apply_MC();
					AI2.apply_MC();
				}
			}
		
		}

	if (!gameOver)
	{

		//---------------------------- O move --------------------------
		actionO = AI2.playNextMove();
		moveResult = env.Board_Move(actionO, OMARK);
		curState = env.getState();
	
			if (moveResult == OMARK) // O wins
			{
				AI1.add_MC_step(curState,-1);
				AI2.add_MC_step(curState, 1);
				AI1.apply_MC();
				AI2.apply_MC();
				gameOver = 1;
			}
			else if (moveResult == 0) // Valid move but no one won
			{
				AI1.add_MC_step(curState, 0);
				AI2.add_MC_step(curState, 0);
				if (env.isFull())
				{
					AI1.apply_MC();
					AI2.apply_MC();
				}
			}
		}
	}
	
	istraining = 0;
	env.resetBoard();
	gameOver = 0;
	AI1.resetState();
	AI2.resetState();
	AI1.reset_MC();
	AI2.reset_MC();
	AI1.saveValues("X_Training.csv");
	AI2.saveValues("O_Training.csv");
	AI1.setEps(0.0);
	AI2.setEps(0.0);
	tf->EditStatsText("Finished Training");

}

void Controller::TilePressHandler(wxCommandEvent& event)
{
	int i,index;
	int moveResult;

	// -------------------human turn ---------------------
	if(!gameOver)
	{ 
		int AIaction;
		for (i = 0; i < tf->buttonIDs.size(); i++)
			if (event.GetId() == tf->buttonIDs[i])
			{ 
				moveResult = env.Board_Move(i, XMARK);
				index = i;
			}

		if (moveResult == -1)
		{
			// Invalid move. Do nothing
		}
		else
		{
			tf->changeButtonImage(index, XMARK);

			if (env.isWinning(XMARK)) // check if human player won
			{
			// display that human wins and give AI negative reward for losing
				gameOver = 1;
				tf->displayWinning(1);
				AI2.add_MC_step(env.getState(), -1);
				AI2.apply_MC();
			}
			else
			{ 

	// ---------------------AI turn -----------------------
			// take action and update state for AI
				AI2.add_MC_step(env.getState(), 0);
				AIaction = AI2.playNextMove();
				env.Board_Move(AIaction, OMARK);
				tf->changeButtonImage(AIaction, OMARK);
			}

			if (env.isWinning(OMARK)) // check if AI won
			{
			// AI won, so give it positive reward
				gameOver = 1;
				tf->displayWinning(2);
				AI2.add_MC_step(env.getState(), 1);
				AI2.apply_MC();
			}
			else
			{
				AI2.add_MC_step(env.getState(), 0);
			}

			if (env.isFull()) // if draw, record data in experience replay buffer
				AI2.apply_MC();

		}

	}

}	

void Controller::ResetButtonHandler(wxCommandEvent& event)
{
	env.resetBoard();
	tf->resetWorld();
	AI1.resetState();
	AI2.resetState();
	AI1.reset_MC();
	AI2.reset_MC();
	gameOver = 0;
}

Controller* MainControl;

class MyApp : public wxApp
{
public:

	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	// create the main application window
	MainControl = new Controller();

	return true;
}