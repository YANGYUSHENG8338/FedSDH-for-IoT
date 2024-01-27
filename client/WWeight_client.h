#pragma once
#ifndef WWEIGHT_CLIENT_H
#define WWEIGHT_CLIENT_H
// Include files
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
#include "SDH_client.h"
#include "WWeight_client.h"
#include "uniqLabels_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
std::tuple<MatrixXd*> WWeight_client(MatrixXd* tr_dat, MatrixXi* tr_labels, double lambda, string SwitchMethod);
#endif