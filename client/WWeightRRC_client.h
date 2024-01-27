#pragma once
#ifndef WWEIGHTRRC_CLIENT_H
#define WWEIGHTRRC_CLIENT_H
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
#include <Eigen/Cholesky>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "LoadData_client.h"
#include "FileExists_client.h"
#include "uniqLabels_client.h"
#include "WWeightRRC_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
std::tuple<MatrixXd*, std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point> WWeightRRC_client(MatrixXd* tr_dat, MatrixXi* tr_labels, double lambda, double mu, string str_EXP, string str_ITR, string SwitchMethod, string weightPoolPath);
#endif
