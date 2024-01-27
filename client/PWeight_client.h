#pragma once
#ifndef PWEIGHT_CLIENT_H
#define PWEIGHT_CLIENT_H
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
#include "PWeight_client.h"
#include "uniqLabels_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;

std::tuple<MatrixXd*> PWeight_client(MatrixXd* tr_dat, MatrixXi* tr_labels, double lambda, int LabelCount);
#endif