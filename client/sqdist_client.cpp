/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�
	Input Parameters
		----------
		X*     : array*; Nsqd1 - by - D; �x�}1
		Y*     : array*; Nsqd2 - by - D; �x�}2
	Output Parameters
		----------
		sqd*   : array*; Nsqd1 - by - Nsqd2; . �ڦ��Z��


*/
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "sqdist_client.h"
#include "SDHWrapper_client.h"
using namespace Eigen;
using namespace std;
using namespace std::chrono; // nanoseconds, system_clock, seconds
using namespace cv;


std::tuple<MatrixXd*> sqdist_client(MatrixXd* X, MatrixXd* Y)
{
    VectorXd* x         = new VectorXd(X->array().square().rowwise().sum());
    RowVectorXd* y      = new RowVectorXd(Y->array().square().rowwise().sum().transpose().eval());
    MatrixXd* first_sqd = new MatrixXd(-2 * (*X) * Y->transpose().eval());//�Ӯɸ��[
    MatrixXd* sqd       = new MatrixXd(MatrixXd::Zero(first_sqd->rows(), first_sqd->cols()));
    (*first_sqd).rowwise() += *y;
    (*first_sqd).colwise() += *x;
    (*sqd) = first_sqd->cwiseMax(0);

    delete first_sqd;
    return std::make_tuple(sqd);
}

