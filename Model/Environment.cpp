#include "Environment.h"

int Environment::isWinning(int markType)
{
	int i;

	std::vector<short> tBoard = Board;

	for(i = 0; i < tBoard.size(); i++)
		if (tBoard[i] != markType)
			tBoard[i] = 0;

	if ( (tBoard[0] && tBoard[4] && tBoard[8]) || (tBoard[2] && tBoard[4] && tBoard[6])) // check for diagonals
		return 1;
	
	for (i = 0; i < 3; i++)// check for full rows or columns
		if ( tBoard[i] && tBoard[i + 3] && tBoard[i + 6])
			return 1;

	for(i=0;i<9;i+=3)
		if(tBoard[i] && tBoard[i + 1] && tBoard[i + 2])
			return 1;

	return 0; // did not win yet
}

Environment::Environment()
{
	state = 0;
	Board = std::vector<short>(9, 0);
}

int Environment::Board_Move(short position, int markType)
{
	if (Board[position] == 0) // empty
	{
		Board[position] = markType;
	}
	else
		return -1;

	if (isWinning(markType))
	{ 
		return markType;
	}
	

	return 0;
}

int Environment::resetBoard()
{
	int i = 0;
	for (i = 0; i < Board.size(); i++)
		Board[i] = 0;
	Bot.resetState();
	return 0;
}
std::vector<short> Environment::getBoard()
{
	return Board;
}
int Environment::getState()
{ // get encoded board state
	int state = 0;
	int i;
	int power=1;
	for (i = 0; i < Board.size(); i++)
	{
	state += Board[i] * power;
	power *= 3;
	}
		return state;
}

int Environment::isFull()
{
	for (int i = 0; i < Board.size(); i++)
		if (Board[i] == 0)
			return 0;

	return 1;
}