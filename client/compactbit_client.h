#pragma once
#ifndef COMPACTBIT_CLIENT_H
#define COMPACTBIT_CLIENT_H
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
std::tuple<MatrixXi*> compactbit_client(MatrixXi* b_ptr);
#endif