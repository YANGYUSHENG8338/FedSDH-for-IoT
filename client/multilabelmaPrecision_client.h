#pragma once
#ifndef MULTILABELMAPRECISION_CLIENT_H
#define MULTILABELMAPRECISION_CLIENT_H
// Include files
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <bitset>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
using namespace Eigen;
using namespace std;
using namespace cv;
std::tuple <double> multilabelmaPrecision_client(MatrixXi Ytrue, MatrixXi Ypred);
#endif