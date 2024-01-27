#pragma once
#ifndef FILEEXISTS_CLIENT_H
#define FILEEXISTS_CLIENT_H
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
std::tuple<bool> FileExists_client(const string& path);

#endif