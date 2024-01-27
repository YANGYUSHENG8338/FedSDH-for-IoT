#pragma once
#ifndef DISPMSG_CLIENT_H
#define DISPMSG_CLIENT_H
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
std::tuple<FILE*> dispMsg_client(const string& disp2filePath);
//std::tuple<FILE*, streambuf*> dispMsg_client(const string& disp2filePath);

#endif