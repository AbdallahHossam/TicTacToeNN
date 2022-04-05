#pragma once
#include "FC.h"
#include <vector>
enum class act_fn {
	RELU, TANH, SIGMOID
};

Eigen::VectorXf net_relu(Eigen::VectorXf x); // relu activation function
Eigen::VectorXf net_reluDerivative(Eigen::VectorXf x); // derivative of relu activation function

Eigen::VectorXf net_tanh(Eigen::VectorXf x); // tanh activation function
Eigen::VectorXf net_tanhDerivative(Eigen::VectorXf x); // derivative of tanh activation function

class NeuralNet
{
private:
	std::vector<FC> layers; // vector of layers of the neural network

	float learning_rate=0.001;
	float t = 1; // counter for adam updates

public:
	NeuralNet();
	NeuralNet(float learning_rate);
	bool AddFCLayer(int n_prev, int n_h, act_fn fn);

	// forward propagation of input through NN
	Eigen::VectorXf forward_prop(Eigen::VectorXf input); 

	// backward propagation given output error (assuming 2 norm loss function)
	void batch_back_prop(Eigen::VectorXf da_L); 

	// batch train neural network on set of inputs and outputs
	Eigen::MatrixXf batchAdamTrainOnSet(Eigen::MatrixXf inputs, Eigen::MatrixXf outputs);

	void setLearningRate(float lr);
	float getLearningRate();

	void printws();
	void printbs();

	void batch_update_weights_adam();

	void setWi(Eigen::MatrixXf wi, int layer);
	void setBi(Eigen::VectorXf bi, int layer);

	Eigen::MatrixXf getWi(int i);
	Eigen::VectorXf getBi(int i);
	int getNumLayers();
	void resetAllWeights();

};
