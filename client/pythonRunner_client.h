#pragma once
#ifndef PYTHONRUNNER_CLIENT_H
#define PYTHONRUNNER_CLIENT_H
#ifdef _WIN64 // Windows x64 環境下使用
	#include <windows.h>
#else// Linux 環境下使用
	#include <unistd.h>
	#include <sys/wait.h>
#endif
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <tuple>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "pythonRunner_client.h"

//int run_python_script(const std::string& script_path);
std::tuple<int> pythonRunner_client(const std::string& script_path_1,const std::string& script_path_2);
#endif // PYTHON_RUNNER_H
