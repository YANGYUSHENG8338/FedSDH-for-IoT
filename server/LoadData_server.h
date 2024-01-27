#pragma once
#ifndef LOADDATA_H
#define LOADDATA_H
// Include files
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

std::tuple<MatrixXd>  LoadData_server(string path);

#endif
