#pragma once
#ifndef MEANSTDPREAGGREITER_CLIENT_H
#define MEANSTDPREAGGREITER_CLIENT_H
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
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
std::tuple<double, double>  meanstdPerAggreIter_client(vector<double> Eva_SDH_num, int aggreItr);
#endif