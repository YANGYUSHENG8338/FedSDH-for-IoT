#pragma once
#ifndef COMPARELABLECLASS_SERVER_H
#define COMPARELABLECLASS_SERVER_H
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
tuple< MatrixXd> compareLableClass_server(MatrixXd Lable1Class, MatrixXd Lable2Class);
#endif