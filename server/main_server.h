#pragma once
#ifndef MAIN_SERVER_H
#define MAIN_SERVER_H
// Include files
#include <stdio.h>
#include <iostream>
#include <thread>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cstdlib>
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
#include "splitData_server.h"
#include "LoadData_server.h"
#include "FileExists_server.h"
#include "int2txt_server.h"
#include "double2txt_server.h"
#include "EmptyWeightPool_server.h"
#include "SendOrReturnWeight_server.h"
#include "GetCurrentTimeStamp_server.h"
#include "compareLableClass_server.h"
#include "AggregatedWWeight_server.h"
#include "main_server.h"

using namespace Eigen;
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
namespace fs = std::filesystem;

int main(int argc, char** argv);

#endif
// End of code generation (main.h)
