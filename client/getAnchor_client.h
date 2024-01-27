#pragma once
#ifndef GETANCHOR_CLIENT_H
#define GETANCHOR_CLIENT_H
// Include files
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
std::tuple<MatrixXd*, MatrixXd*> getAnchor_client(MatrixXd* Xtran, double nAnch);

#endif
