#pragma once
#ifndef DOUBLE2TXT_CLIENT_H
#define DOUBLE2TXT_CLIENT_H
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
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
std::tuple< > double2txt_client(Eigen::MatrixXd* mat, string path);

#endif