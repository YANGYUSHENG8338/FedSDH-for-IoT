#pragma once
#ifndef SQDIST_CLIENT_H
#define SQDIST_CLIENT_H
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
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "SDHWrapper_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::chrono; // nanoseconds, system_clock, seconds

std::tuple<MatrixXd*> sqdist_client(MatrixXd* X, MatrixXd* Y);
#endif