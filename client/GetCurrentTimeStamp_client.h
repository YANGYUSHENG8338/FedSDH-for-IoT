#pragma once
#ifndef GETCURRENTTIMESTAMP_CLIENT_H
#define GETCURRENTTIMESTAMP_CLIENT_H
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
std::tuple<string> GetCurrentTimeStamp_client(int time_stamp_type);
#endif