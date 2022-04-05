#include "Agent.h"


Agent::Agent()
{
	int i;
	//for (i = 0; i < NUM_STATES; i++)
	//	Va[i] = 0;

	net = NeuralNet(INIT_LEARNING_RATE);

	net.AddFCLayer(9, 50, act_fn::TANH); // 3 layer net
	net.AddFCLayer(50, 30, act_fn::RELU);
	net.AddFCLayer(30, 1, act_fn::TANH); // tanh to better represent returns, which have a range from -1 to 1

	gen = std::mt19937(rd());
	uni_rand = std::uniform_real_distribution<>(0.0, 1.0);
	curState = 0;
	markType = 1;
	discount = 0.9;
	step_size = 0.2;
	epsilon = 0.0; // orignally 0.1 (while training)
}
// returns set of valid actions
std::vector<unsigned short> Agent::getpossibleActions(int curState)
{
	int temp_state = curState;
	std::vector<unsigned short> actions;
	int i;
	int remainder;
	for (i = 0; i < 9; i++)
	{
		remainder = temp_state % 3;

		if (remainder == 0)
			actions.push_back(i);

		temp_state = (temp_state - remainder) / 3;

	}
	return actions;
}

int Agent::getNextState(int curState, int action)
{
	std::vector <unsigned short> act = getpossibleActions(curState);
	int i;
	int action_possible = 0;

	for (i = 0; i < act.size(); i++)
		if (act[i] == action)
			action_possible = 1;

	if (!action_possible || action < 0 || action > 8) // invalid action
		return -1;

	int inc = pow(3, action);
	return curState + inc * markType;
}
// chooses the action with the maximum aftervalue
int Agent::argmaxNextAction()
{
	std::vector <unsigned short> act = getpossibleActions(curState);
	std::vector <unsigned short> ties(0); // handle ties using this variable
	float max_v = -std::numeric_limits<float>::max();
	int curAV; // current after value
	int maxAct = -1; // action with maximum after value
	int i;
	ties.clear();
	for (i = 0; i < act.size(); i++)
	{
		curAV = getNextState(curState, act[i]);
		float curV = net.forward_prop(indToVector(curAV))(0);
		if (curV > max_v)
		{
			max_v = curV;
			maxAct = act[i];
			ties.clear();
			ties.push_back(maxAct);
		}
		else if (curV == max_v)
		{
			ties.push_back(act[i]);
		}
	}
	if (ties.size() > 1)
	{
		return ties[rand() % ties.size()];
	}else
		return maxAct;
}

int Agent::epsGreedy()
{

	double prob = uni_rand(gen);

	if (prob > epsilon)// take greedy action
	{
		int max_action = argmaxNextAction();
		return max_action;
	}
	else // take random action
	{
		std::vector <unsigned short> act = getpossibleActions(curState);
		std::uniform_real_distribution<float> distribution2(0.0, 1.0);
		float dist_val = uni_rand(gen);
		int selection = (int)(dist_val * (float)act.size());
		if (act.size() > 0)
			return act[selection];
		else return -1;
	}

}

int Agent::playNextMove() // after human's turn
{
	// update afterstate value function:
//	Va[curState] = Va[curState] + step_size * (reward + discount * Va[newState] - Va[curState]);
	// update current state
//	curState = newState;
	// select epsilon greedy action:
	return epsGreedy();

}

void Agent::add_MC_step(int newState, float reward)
{
	states.push_back(newState);
	rewards.push_back(reward);

	curState = newState;
}

// calculate after value returns, apply batch learning on MC, then clear episode data
void Agent::apply_MC()
{
//	Eigen::MatrixXf inputs(9, states.size()); // create matrix of inputs, each column is a state
//	Eigen::MatrixXf outputs(1, states.size()); // create matrix of output returns over episode
	returns.resize(rewards.size(),0);
	returns.back() = rewards.back();

	
	// calculate returns and set outputs and input matrices for learning
	for (int i = rewards.size()-2; i >= 0; i--) // -2 not -1
	{
		returns[i] = discount * returns[i + 1] + rewards[i];
//		outputs.col(i)(0) = returns[i];
//		inputs.col(i) = indToVector(states[i]);
	}
	

	for (int i = 0; i < states.size(); i++)
	{
		int ind = getIndInStateReplay(states[i]);
		if (ind == -1)
		{
			state_replay.push_back(states[i]);
			return_replay.push_back(returns[i]);
			no_visits_replay.push_back(1);
		}
		else if (ind>=0)
		{
			return_replay[ind]+=returns[i]; // accumulate returns
			no_visits_replay[ind]++; // increment number of visits
		}
		else
		{
			std::cout << "Error!";
		}
	}


//	net.batchAdamTrainOnSet(inputs, outputs);
	curState = 0;
	states.clear();
	rewards.clear();
	returns.clear();
	
}

void Agent::learn_replay()
{
	// swap experience replay vectors

	// Fisher-Yates Shuffle
	int n = state_replay.size();

	for (int i = 0; i < n; i++)
	{
		int j = i + rand() % (n - i);
		std::swap(state_replay[i], state_replay[j]);
		std::swap(return_replay[i], return_replay[j]);
		std::swap(no_visits_replay[i], no_visits_replay[j]);
	}

	// learn on mini-batches
	std::vector<int> state_miniB;
	std::vector<float> return_miniB;

	// collect mini batches and learn
	for (int i = 0; i < n; )
	{
		int j;
		//collect one batch

		for (j = i; j < n && (j-i)<MINI_BATCH_SIZE; j++)
		{
			if(no_visits_replay[j]>0)
			{
				state_miniB.push_back(state_replay[j]);
				return_miniB.push_back(return_replay[j]/no_visits_replay[j]);
			}
		}

		Eigen::MatrixXf inputs(9, state_miniB.size()); // create matrix of inputs, each column is a state
		Eigen::MatrixXf outputs(1, state_miniB.size()); // create matrix of output returns over episode

		for (int k = 0; k < state_miniB.size(); k++)
		{
			outputs.col(k)(0) = return_miniB[k];
			inputs.col(k) = indToVector(state_miniB[k]);
		}

		net.batchAdamTrainOnSet(inputs, outputs);

		state_miniB.clear();
		return_miniB.clear();

		i = j;
	}

	errors.push_back(calculate_RMSE());

}



void Agent::reset_MC()
{
	states.clear();
	rewards.clear();
	returns.clear();
}

int Agent::getIndInStateReplay(int state)
{
	if (state < 0)
		return -2;// invalid state

	for (int i = 0; i < state_replay.size(); i++)
		if (state_replay[i] == state)
			return i;

	return -1;
}

void Agent::resetState()
{
	curState = 0;
	states.clear();
	rewards.clear();
	returns.clear();

}

int Agent::loadValues(string file)
{
	std::ifstream fileIn(file);

	//	fileIn.open(file, std::ios::in);

	if (!fileIn)
		return -1;
	int i, layer;
	string line, value, temp;
	i = 0;
	layer = 0;

	while (fileIn && layer<net.getNumLayers())
	{
		string s,s2;
		if (!getline(fileIn, s))
			break;

		std::istringstream ss(s); // ss holds one line (each line is a different matrix)

		Eigen::MatrixXf wi = net.getWi(layer); // read w

		int num_params = wi.rows()*wi.cols();
		
		if (!getline(ss, s2, ','))
			break; // get first element, which is number of parameters in line

		if (num_params != std::stoi(s2))
		{	// incorrect data size
			net.resetAllWeights(); // remove any initialized params as they might be incorrect
			return -1; 
		}

		for (int i=0;ss && i < wi.rows();i++)
		{
			for (int j = 0; ss && j < wi.cols(); j++)
			{
				if (!getline(ss, s2, ','))
					break;

				wi(i, j) = std::stof(s2);
			}

		}
		// read bias vector -----------------------

		if (!getline(fileIn, s))
			break;

		ss = std::istringstream(s); // ss holds one line (each line is a different matrix)

		Eigen::VectorXf bi = net.getBi(layer); // read b

		num_params = bi.rows();

		if (!getline(ss, s2, ','))
			break; // get first element, which is number of parameters in line

		if (num_params != std::stoi(s2))
		{	// incorrect data size
			net.resetAllWeights(); // remove any initialized params as they might be incorrect
			return -1; 
		}

		for (int i = 0; ss && i < bi.rows(); i++)
		{
			if (!getline(ss, s2, ','))
				break;

			bi(i) = std::stof(s2);
		}

		net.setWi(wi, layer);
		net.setBi(bi, layer);

		layer++;

	}

	/*
	while (fileIn)
	{
		string s;
		if (!getline(fileIn, s))break;
		std::istringstream ss(s);

		while (ss && i < NUM_STATES)
		{
			string s2;

			if (!getline(ss, s2, ','))
				break;

			Va[i] = std::stof(s2);
			i++;
		}

	}*/

//	if (!fileIn.eof())
//		return -2;

	fileIn.close();
}

int Agent::saveValues(string file)
{
	std::ofstream fileOut(file);

	if (!fileOut) // did not open file successfully
		return -1;

	for (int i = 0; i < net.getNumLayers();i++)
	{

		Eigen::MatrixXf wi = net.getWi(i);
		Eigen::VectorXf bi = net.getBi(i);

		fileOut << wi.rows() * wi.cols();

		for (int j = 0; j < wi.rows(); j++)
		{
			for (int k = 0; k < wi.cols(); k++)
			{
				fileOut << "," << (wi(j,k));
			}
		}
		fileOut << "\n";

		fileOut << bi.rows();
		for (int j = 0; j < bi.rows(); j++)
			fileOut << "," << bi(j);

		fileOut << "\n";

	}

}

void Agent::setMark(int markType)
{
	this->markType = markType;
}


/*
float Agent::getValue(int i)
{
	return Va[i];
}
*/

// unlike the convention used between environment and agent(empty=0, X=1, O=2, this function normalizes NN inputs
// (empty = 0, X=1, O=-1)
Eigen::VectorXf indToVector(int index)
{
	int tmp = index;
	Eigen::VectorXf x(9);
	for (int i = 0; i < 9; i++)
	{
		x(i) = tmp % 3;
		tmp /= 3;
	}

	// change Os to -1
	for (int i = 0; i < 9; i++)
		if (x(i) == 2)
			x(i) = -1;

	return x;
}

void Agent::setEps(float eps)
{
	this->epsilon = eps;
}

void Agent::setLearningRate(float lr)
{
	this->net.setLearningRate(lr);
}

float Agent::getLearningRate()
{
	return this->net.getLearningRate();
}

float Agent::calculate_RMSE()
{
	float err = 0;
	for(int i=0;i<state_replay.size();i++)
	{ 
		err += pow(return_replay[i]/no_visits_replay[i]- net.forward_prop(indToVector(state_replay[i]))(0),2)/2;
	}
	return err/state_replay.size();
}

std::vector<float> Agent::getRMSE()
{
	return errors;
}

void Agent::resetNN()
{
	// reset the neural network

	net.resetAllWeights();
	net.setLearningRate(INIT_LEARNING_RATE);
}

void Agent::resetHistory()
{
	state_replay.clear();
	no_visits_replay.clear();
	return_replay.clear();
}
