#pragma once
#ifndef LOADDATA_CLIENT_H
#define LOADDATA_CLIENT_H
// Include files
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

std::tuple<MatrixXd>  LoadData_client(string path);

#endif
// End of code generation (Fixedpoint.h)