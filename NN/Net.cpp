#include "Net.h"
#include <iostream>

NeuralNet::NeuralNet()
{
	learning_rate = 0.01;
	t = 1;
}

NeuralNet::NeuralNet(float learning_rate)
{
	this->learning_rate = learning_rate;
	t = 1;
}

bool NeuralNet::AddFCLayer(int n_prev, int n_h, act_fn fn)
{
	if(layers.size()!=0)
		if (layers[layers.size() - 1].get_nh() != n_prev)
			return false;
	switch(fn)
	{

	case act_fn::RELU:
		layers.push_back(FC(n_prev, n_h, &net_relu, &net_reluDerivative));
	break;

	case act_fn::TANH:
		layers.push_back(FC(n_prev, n_h, &net_tanh, &net_tanhDerivative));
	break;

	}
	return true;
}

Eigen::VectorXf NeuralNet::forward_prop(Eigen::VectorXf Inputs)
{
	Eigen::VectorXf a_l;
	a_l = Inputs;
	for (int i = 0; i < layers.size(); i++)
	{
		a_l = layers[i].forward_prop(a_l);
	}
	return a_l;
}

Eigen::MatrixXf NeuralNet::batchAdamTrainOnSet(Eigen::MatrixXf inputs, Eigen::MatrixXf outputs)
{
	Eigen::MatrixXf out_hat(outputs.rows(), outputs.cols());
	for (int m = 0; m < inputs.cols(); m++) // iterate over m examples
	{
		Eigen::VectorXf x_m = inputs.col(m);
		Eigen::VectorXf y_m = outputs.col(m);

		Eigen::VectorXf y_hat_m = forward_prop(x_m);
		out_hat.col(m) = y_hat_m;
		batch_back_prop(y_hat_m - y_m);

	}
	batch_update_weights_adam();
	return out_hat;

}

void NeuralNet::setLearningRate(float lr)
{
	this->learning_rate = lr;
}

float NeuralNet::getLearningRate()
{
	return learning_rate;
}

void NeuralNet::printws()
{
	for (int i = 0; i < layers.size(); i++)
		std::cout << "\n\nW[" << i << "]:\n" << layers[i].getw();
}

void NeuralNet::printbs()
{
	for (int i = 0; i < layers.size(); i++)
		std::cout << "\n\nb[" << i << "]:\n" << layers[i].getb();
}

void NeuralNet::batch_back_prop(Eigen::VectorXf da_L)
{
	Eigen::VectorXf da_l;
	da_l = da_L;
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		da_l = layers[i].batch_backward_prop(da_l);
	}
}

void NeuralNet::batch_update_weights_adam()
{
	for (int i = 0; i < layers.size(); i++)
		layers[i].batch_adam_step(learning_rate,t);

	if (t < 1e6)
		t++;
}

void NeuralNet::setWi(Eigen::MatrixXf wi, int layer)
{
	layers[layer].setW(wi);
}

void NeuralNet::setBi(Eigen::VectorXf bi, int layer)
{

	layers[layer].setB(bi);
}

Eigen::MatrixXf NeuralNet::getWi(int i)
{
	if (i < layers.size())
		return layers[i].getw();
	else 
		return Eigen::MatrixXf::Zero(1,1);
}

Eigen::VectorXf NeuralNet::getBi(int i)
{
	if (i < layers.size())
		return layers[i].getb();
	else
		return Eigen::VectorXf::Zero(1);
}

int NeuralNet::getNumLayers()
{
	return layers.size();
}

void NeuralNet::resetAllWeights()
{
	for (int i = 0; i < layers.size(); i++)
		layers[i].reset();
}

Eigen::VectorXf net_relu(Eigen::VectorXf x)
{
	Eigen::VectorXf x_relu(x.rows());
	for (int i = 0; i < x.rows(); i++)
		x_relu[i] = (x[i] >= 1e-4*x[i] ? x[i] : 1e-4 * x[i]);

	return x_relu;
}
Eigen::VectorXf net_reluDerivative(Eigen::VectorXf x)
{
	Eigen::VectorXf reluD(x.rows());
	for (int i = 0; i < reluD.rows(); i++)
		reluD[i] = (x[i] >= 1e-4 * x[i] ? 1 : 1e-4);
	return reluD;
}

Eigen::VectorXf net_tanh(Eigen::VectorXf x)
{
	Eigen::VectorXf res(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++)
		res(i) = tanh(x(i));

	return res;
}

Eigen::VectorXf net_tanhDerivative(Eigen::VectorXf x)
{
	Eigen::VectorXf res(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++)
		res(i) = 1- tanh(x(i))* tanh(x(i));

	return res;
}
