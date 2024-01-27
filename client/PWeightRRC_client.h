#pragma once
#ifndef PWEIGHTRRC_CLIENT_H
#define PWEIGHTRRC_CLIENT_H
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
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "LoadData_client.h"
#include "SDH_client.h"
#include "PWeightRRC_client.h"
#include "uniqLabels_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;


std::tuple<MatrixXd*> PWeightRRC_client(MatrixXd* tr_dat, MatrixXi* tr_labels, double nu, double mu, string str_EXP, string str_ITR, string SwitchMethod, string LoadDataPath, int LabelCount);
#endif