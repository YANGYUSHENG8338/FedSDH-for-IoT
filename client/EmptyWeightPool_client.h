#pragma once
#ifndef EMPTYWEIGHTPOOL_CLIENT_H
#define EMPTYWEIGHTPOOL_CLIENT_H
// Include files
#ifdef _WIN64 // Windows x64 ���ҤU�ϥ�
	#include <windows.h>
	#include <filesystem>
#else// Linux ���ҤU�ϥ�
	#include <unistd.h>
	#include <sys/wait.h>
	#include <dirent.h>
#endif
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
std::tuple<> EmptyWeightPool_client(const string& weightPoolPath);

#endif