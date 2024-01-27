#pragma once
#ifndef SENDORRETURNWEIGHT_SERVER_H
#define SENDORRETURNWEIGHT_SERVER_H
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
#include <chrono>
#include <thread>
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
std::tuple<> SendOrReturnWeight_server(const string& SendOrReturnWeight);

#endif