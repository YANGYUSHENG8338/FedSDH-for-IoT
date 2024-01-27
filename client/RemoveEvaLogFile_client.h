#pragma once
#ifndef REMOVEEVALOGFILE_CLIENT_H
#define REMOVEEVALOGFILE_CLIENT_H
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
std::tuple<> RemoveEvaLogFile_client(const string& evaPoolPath, const string& SwitchSegment, const string& SwitchEvaluation);

#endif