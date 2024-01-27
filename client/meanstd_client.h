#pragma once
#ifndef MEANSTD_CLIENT_H
#define MEANSTD_CLIENT_H
// Include files
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <ctime>
#include <numeric>
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
std::tuple<double, double>  meanstd_client(vector<double> Eva_num);
#endif