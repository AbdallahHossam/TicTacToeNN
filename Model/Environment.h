#pragma once
#include <vector>
#include "Agent.h"
unsigned int boardToState(std::vector<short> board); // takes board array of 9 elements and return state number
std::vector<short> stateToBoard(unsigned int State); // takes state and returns an array of size 9
#define XMARK 1
#define OMARK 2
class Environment
{
private:

	int state;
	std::vector<short> Board;
	int terminal_state;
	Agent Bot;
public:
	Environment();
	int isWinning(int markType);
//	int AIMove(int newState, float reward); // AI action after human's move
//	void updateAI(int newState, float reward); // After its own move
//	void finalUpdateAI(float reward);
	int Board_Move(short position, int markType);
	std::vector<short> getBoard();
	int getState();
	int resetBoard();
	int isFull();
};

