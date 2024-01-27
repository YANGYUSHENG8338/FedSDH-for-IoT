#pragma once
#ifndef UNIQLABELS_SERVER_H
#define UNIQLABELS_SERVER_H
// Include files
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
std::tuple<vector<double>, vector<double>::iterator > uniqLabels_server(VectorXd Eigen_Label);

#endif