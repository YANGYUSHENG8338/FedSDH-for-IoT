#pragma once
#ifndef INT2TXT_SERVER_H
#define INT2TXT_SERVER_H
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
std::tuple< > int2txt_server(Eigen::MatrixXi mat, string path);

#endif