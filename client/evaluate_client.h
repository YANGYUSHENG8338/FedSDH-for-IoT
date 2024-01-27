#pragma once
#ifndef EVALUATE_CLIENT_H
#define EVALUATE_CLIENT_H
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
using namespace Eigen;
using namespace std;
std::tuple < float, float, float, float > evaluate_client(MatrixXi Rfact, MatrixXi Rpred, string method);
#endif