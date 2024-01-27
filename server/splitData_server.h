#pragma once
#ifndef SPLITDATA_SERVER_H
#define SPLITDATA_SERVER_H
// Include files
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "uniqLabels_server.h"
#include "splitData_server.h"
using namespace std;
using namespace Eigen;
std::tuple< MatrixXd, MatrixXd, MatrixXd, MatrixXd, MatrixXd, MatrixXd, int> splitData_server(string datasetnam, MatrixXd X, VectorXd Eigen_Label, double XtranRatio, double XtuneRatio, double XtestRatio);

#endif

