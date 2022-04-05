#pragma once
#include <vector>
#include <random>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../NN/Net.h"



//#define NUM_STATES 19682

#define MINI_BATCH_SIZE 512 // mini batch size
#define INIT_LEARNING_RATE 0.05 // initial learning rate

using std::string;
using std::fstream;
// An agent that uses Monte Carlo with experience replay to play TicTacToe
// Mini-batch with Adam optimizer is used
class Agent
{
private:
	//float Va[NUM_STATES]; // after values for each possible state of the board
	
	// Use a Neural Network instead :D
	NeuralNet net;

	std::vector<int> states; // state sequence for the current episode
	std::vector<float> rewards; // reward sequence for the current episode
	std::vector<float> returns; // returns for the current episode

	std::vector<int> state_replay; // states visited and recorded in history encripted format
	std::vector<float> no_visits_replay; // no_visits_replay[i] counts number of visits for state_replay[i]
	std::vector<float> return_replay; // return_replay[i] contains sum of returns for state state_replay[i]
	
	std::vector<float> errors; // error recorded every history replay


	int curState; // current state encoding
	float discount; // discount
	float epsilon; // epsilon parameter for epsilon-greedy agent

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen;
	std::uniform_real_distribution<> uni_rand;

	std::vector<unsigned short> getpossibleActions(int curState);

	// get action no.
	int argmaxNextAction();

	// returns next state value given current state and action
	int getNextState(int curState, int action); 

	// selects a random action with probability epsilon, otherwise selects greedy action 
	int epsGreedy(); 

public:

	// X or O player
	int markType;

	Agent();

	// set whether the agent plays as X or O
	void setMark(int markType);

	// loads neural network weights from a csv file
	int loadValues(string file);

	// saves neural network weights in a csv file
	int saveValues(string file);
	

	// Record reward received using Monte Carlo on transition to the new state
	void add_MC_step(int newState, float reward); 

	// Record rewards for the current episode using Monte Carlo
	void apply_MC(); 

	// Learn from replay data of MC and update the after value predictions
	void learn_replay(); 

	// clear state, reward and return vectors related to Monte Carlo
	void reset_MC();
	
	// returns index of state encoding (state) as in vector state_replay
	int getIndInStateReplay(int state);
	
	// returns action No. based on epsilon greedy policy
	int playNextMove();

	void resetState();

	// sets epsilon
	void setEps(float eps);

	void setLearningRate(float lr);

	float getLearningRate();

	float calculate_RMSE();

	std::vector<float> getRMSE();

	// resets all weights for the neural network
	void resetNN();

	void resetHistory();

};

Eigen::VectorXf indToVector(int index);
