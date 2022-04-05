#include "FC.h"

FC::FC(int n_prev, int n_h, Eigen::VectorXf(*activation_fn)(Eigen::VectorXf z), Eigen::VectorXf(*act_derivative_fn)(Eigen::VectorXf z))
{
	this->n_prev = n_prev;
	this->n_h = n_h;
	w = Eigen::MatrixXf::Random(n_h, n_prev) * (sqrt(2.0/n_prev)); 
	b = Eigen::VectorXf::Zero(n_h);
	dw = Eigen::MatrixXf::Zero(n_h, n_prev);
	db = Eigen::VectorXf::Zero(n_h);

	vdw = Eigen::MatrixXf::Zero(n_h, n_prev);
	vdb = Eigen::VectorXf::Zero(n_h);

	vdw_corr = Eigen::MatrixXf::Zero(n_h, n_prev);
	vdb_corr = Eigen::VectorXf::Zero(n_h);

	sdw = Eigen::MatrixXf::Zero(n_h, n_prev);
	sdb = Eigen::VectorXf::Zero(n_h);

	sdw_corr = Eigen::MatrixXf::Zero(n_h, n_prev);
	sdb_corr = Eigen::VectorXf::Zero(n_h);



	dw_buff = Eigen::MatrixXf::Zero(n_h, n_prev);
	db_buff = Eigen::VectorXf::Zero(n_h);
	buff = 0;

	a = Eigen::VectorXf::Zero(n_h);
	z = Eigen::VectorXf::Zero(n_h);
	this->activation_fn = activation_fn;
	this->act_derivative_fn = act_derivative_fn;
}

int FC::get_nh()
{
	return n_h;
}

Eigen::MatrixXf FC::getw()
{
	return w;
}

Eigen::VectorXf FC::getb()
{
	return b;
}

Eigen::VectorXf FC::forward_prop(Eigen::VectorXf a_prev)
{
	this->a_prev = a_prev;
	z = w * a_prev + b;
	a = activation_fn(z);
	return a; // return activations for next layers
}

Eigen::VectorXf FC::backward_prop(Eigen::VectorXf da)
{
	Eigen::VectorXf dz = da.cwiseProduct((*act_derivative_fn)(z));
	dw = dz * a_prev.transpose();
	db = dz;
	Eigen::VectorXf da_prev = w.transpose() * dz;
	return da_prev; // return da_prev for further backprop
}

Eigen::VectorXf FC::batch_backward_prop(Eigen::VectorXf da)
{
	Eigen::VectorXf dz = da.cwiseProduct((*act_derivative_fn)(z));
	dw_buff += dz * a_prev.transpose();
	db_buff += dz;
	buff++;
	Eigen::VectorXf da_prev = w.transpose() * dz;
	return da_prev; // return da_prev for further backprop
}

void FC::gradient_descent_step(float learning_rate)
{
	w -= learning_rate * dw;
	b -= learning_rate * db;
}


void FC::batch_gradient_descent_step(float learning_rate)
{
	if (buff == 0)
		return;
	

	w -= (learning_rate / (float)buff) * dw_buff;
	b -= (learning_rate / (float)buff) * db_buff;


	buff = 0;
	dw_buff = Eigen::MatrixXf::Zero(n_h, n_prev);
	db_buff = Eigen::VectorXf::Zero(n_h);
}


void FC::batch_adam_step(float learning_rate, float t)
{

	if (buff == 0)
		return;

	dw_buff /= (float(buff));
	

	vdw = Beta1 * vdw + (1 - Beta1) * dw_buff;
	vdw_corr = vdw / (1 - pow(Beta1, t));

	sdw = Beta2 * sdw + (1 - Beta2) * dw_buff.cwiseProduct(dw_buff);
	sdw_corr = sdw / (1 - pow(Beta2, t));

	w -= learning_rate * vdw_corr.cwiseQuotient(sdw_corr.cwiseSqrt() + Eigen::MatrixXf::Ones(sdw_corr.rows(),sdw_corr.cols())*eps );


	db_buff /= (float(buff));


	vdb = Beta1 * vdb + (1 - Beta1) * db_buff;
	vdb_corr = vdb / (1 - pow(Beta1, t));

	sdb = Beta2 * sdb + (1 - Beta2) * db_buff.cwiseProduct(db_buff);
	sdb_corr = sdb / (1 - pow(Beta2, t));

	b -= learning_rate * vdb_corr.cwiseQuotient(sdb_corr.cwiseSqrt() + Eigen::VectorXf::Ones(sdb_corr.rows(),1) * eps);


	buff = 0;
	dw_buff = Eigen::MatrixXf::Zero(n_h, n_prev);
	db_buff = Eigen::VectorXf::Zero(n_h);

}

void FC::adam_step(float learning_rate, float t)
{

	vdw = Beta1 * vdw + (1 - Beta1) * dw;
	vdw_corr = vdw / (1 - pow(Beta1, t));

	sdw = Beta2 * sdw + (1 - Beta2) * dw.cwiseProduct(dw);
	sdw_corr = sdw / (1 - pow(Beta2, t));

	w -= learning_rate * vdw_corr.cwiseQuotient(sdw_corr.cwiseSqrt() + Eigen::MatrixXf::Ones(sdw_corr.rows(), sdw_corr.cols()) * eps);

	vdb = Beta1 * vdb + (1 - Beta1) * db;
	vdb_corr = vdb / (1 - pow(Beta1, t));

	sdb = Beta2 * sdb + (1 - Beta2) * db.cwiseProduct(db);
	sdb_corr = sdb / (1 - pow(Beta2, t));

	b -= learning_rate * vdb_corr.cwiseQuotient(sdb_corr.cwiseSqrt() + Eigen::VectorXf::Ones(sdb_corr.rows(), 1) * eps);


}

void FC::reset()
{
	w = Eigen::MatrixXf::Random(n_h, n_prev) * (sqrt(2.0 / n_prev));
	b = Eigen::VectorXf::Zero(n_h);
	dw = Eigen::MatrixXf::Zero(n_h, n_prev);
	db = Eigen::VectorXf::Zero(n_h);

	vdw = Eigen::MatrixXf::Zero(n_h, n_prev);
	vdb = Eigen::VectorXf::Zero(n_h);

	vdw_corr = Eigen::MatrixXf::Zero(n_h, n_prev);
	vdb_corr = Eigen::VectorXf::Zero(n_h);

	sdw = Eigen::MatrixXf::Zero(n_h, n_prev);
	sdb = Eigen::VectorXf::Zero(n_h);

	sdw_corr = Eigen::MatrixXf::Zero(n_h, n_prev);
	sdb_corr = Eigen::VectorXf::Zero(n_h);


	dw_buff = Eigen::MatrixXf::Zero(n_h, n_prev);
	db_buff = Eigen::VectorXf::Zero(n_h);
	buff = 0;

	a = Eigen::VectorXf::Zero(n_h);
	z = Eigen::VectorXf::Zero(n_h);
}

void FC::setW(Eigen::MatrixXf wi)
{
	w = wi;
}

void FC::setB(Eigen::VectorXf bi)
{
	b = bi;
}



