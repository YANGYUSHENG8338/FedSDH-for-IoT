#pragma once
#ifndef MAIN_CLIENT_H
#define MAIN_CLIENT_H
// Include files
#pragma warning( disable : 4996 )
#ifdef _WIN64 // Windows x64 環境下使用
	#include <windows.h>
	#include <filesystem>
#else// Linux 環境下使用
	#include <unistd.h>
	#include <sys/wait.h>
	#include <dirent.h>
#endif
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <ctime>
#include <random>
#include <numeric>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "meanstd_client.h"
#include "meanstdPerAggreIter_client.h"
#include "SDHWrapper_client.h"
#include "getLabel_client.h"
#include "evaluate_client.h"
#include "multilabelmaPrecision_client.h"
#include "getAnchor_client.h"
#include "LoadData_client.h"
#include "FileExists_client.h"
#include "uniqLabels_client.h"
#include "dispMsg_client.h"
#include "double2txt_client.h"
#include "EmptyWeightPool_client.h"
#include "GetCurrentTimeStamp_client.h"
#include "pythonRunner_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
// Function Declarations
int main(int argc, char** argv);

#endif
// End of code generation (main.h)
