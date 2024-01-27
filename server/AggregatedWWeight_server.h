#pragma once
#ifndef AGGREGATEDWWEIGHT_SERVER_H
#define AGGREGATEDWWEIGHT_SERVER_H
// Include files
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <limits>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "compareLableClass_server.h"
#include "AggregatedWWeight_server.h"
using namespace std;
using namespace Eigen;
tuple< MatrixXd, MatrixXd> AggregatedWWeight_server(MatrixXd RENEW_RASP_labelList, MatrixXd RENEW_NANO_labelList, MatrixXd RENEW_RASP_WWeight, MatrixXd RENEW_NANO_WWeight, int LabelCount, int paramL, double raspDataRatio, double nanoDataRatio);
#endif