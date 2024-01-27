#pragma once
#ifndef GETCURRENTTIMESTAMP_SERVER_H
#define GETCURRENTTIMESTAMP_SERVER_H
// Include files
#include <stdio.h>
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
using namespace std::chrono; // nanoseconds, system_clock, seconds
std::tuple<string> GetCurrentTimeStamp_server(int time_stamp_type);
#endif