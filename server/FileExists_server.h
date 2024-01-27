#pragma once
#ifndef FILEEXISTS_SERVER_H
#define FILEEXISTS_SERVER_H
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
std::tuple<bool> FileExists_server(const string& fileName);

#endif