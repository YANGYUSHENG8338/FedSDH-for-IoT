// ###檔頭寫一下 ，示範如何丟參數

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
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "pythonRunner_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;
std::tuple<int> pythonRunner_client(const std::string& script_path_1,const std::string& script_path_2)
{
    #ifdef _WIN64
        // Windows x64 環境下使用
        std::cout << "Running on Windows x64 environment." << std::endl;
        std::cout << script_path_1 << std::endl;
        std::cout << script_path_2 << std::endl;
        int ret = 1;
    #else
        // Linux 環境下使用
        const char* pythonPath = script_path_1.c_str();
        const char* logPath    = script_path_2.c_str();
        write(STDOUT_FILENO, "Hello Linux!\n", 13);
        int ret = 1;
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "Error: failed to fork." << std::endl;
            ret = 0;
        } else if (pid == 0) {
            execl("/usr/bin/python", "python3", pythonPath, logPath, nullptr);
    //        freopen(script_path_1.c_str(), "a", stdout);
            std::cout << "process" << std::endl;
            std::cerr << "Error: failed to execute python." << std::endl;
            ret = 1;
            _exit(1);
        } else {
            std::cout << script_path_1 << std::endl;
            std::cout << script_path_2 << std::endl;
            std::cout << "Parent process is running." << std::endl;
    //        wait(nullptr);
            std::cout << "Parent process is done." << std::endl;
        }
    #endif
    return std::make_tuple(ret);
}

