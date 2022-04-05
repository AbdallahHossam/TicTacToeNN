#pragma once
#include <Eigen/Dense>

// fully connected layer
class FC
{
	
	int n_prev, n_h, n_next; // size of hidden layer units
	
	// variables for batch gradient descent
	int buff;
	Eigen::MatrixXf dw_buff; // derivative of w
	Eigen::VectorXf db_buff; // derivative of b

	float Beta1 = 0.9;
	float Beta2 = 0.999;
	float eps = 1e-8;



	Eigen::MatrixXf vdw; // derivative of w
	Eigen::MatrixXf vdw_corr; // derivative of w

	Eigen::MatrixXf sdw; // derivative of w
	Eigen::MatrixXf sdw_corr; // derivative of w


	Eigen::VectorXf vdb; // derivative of b
	Eigen::VectorXf vdb_corr; // derivative of b

	Eigen::VectorXf sdb; // derivative of b
	Eigen::VectorXf sdb_corr; // derivative of b

	// Weights
	Eigen::MatrixXf w; 
	Eigen::MatrixXf dw; // derivative of w

	// Bias
	Eigen::VectorXf b; 
	Eigen::VectorXf db; // derivative of b

	Eigen::VectorXf z;
	Eigen::VectorXf a;

	Eigen::VectorXf a_prev;
	Eigen::VectorXf(*activation_fn)(Eigen::VectorXf z);
	Eigen::VectorXf(*act_derivative_fn)(Eigen::VectorXf z);




public:
	FC(int n_prev, int n_h, Eigen::VectorXf (*activation_fn)(Eigen::VectorXf z), Eigen::VectorXf(*act_derivative_fn)(Eigen::VectorXf z));
	int get_nh();
	Eigen::MatrixXf getw();
	Eigen::VectorXf getb();


	// Uses previous activations and outputs activation values of current layer
	Eigen::VectorXf forward_prop(Eigen::VectorXf a_prev); 
	
	// uses derviative of current layer activations and calculates weight derivatives 
	Eigen::VectorXf backward_prop(Eigen::VectorXf da); 

	// Uses derviative of current layer activations and appends weight derivatives
	Eigen::VectorXf batch_backward_prop(Eigen::VectorXf da); 

	void gradient_descent_step(float learning_rate);
	void batch_gradient_descent_step(float learning_rate);
	void batch_adam_step(float learning_rate, float t);
	void adam_step(float learning_rate, float t);
	void reset();
	void setW(Eigen::MatrixXf wi);
	void setB(Eigen::VectorXf bi);

};