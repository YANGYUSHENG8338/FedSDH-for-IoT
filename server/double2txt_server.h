#pragma once
#ifndef DOUBLE2TXT_SERVER_H
#define DOUBLE2TXT_SERVER_H
// Include files
#include <stdio.h>
#include <iostream>
#include <filesystem>
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
std::tuple< > double2txt_server(Eigen::MatrixXd mat, string path);

#endif