/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1   Initial
Log: 0.0.2   改成指標形式

***副程式用途***
// 計算每一輪聚合輪次的評估指標(包含 Tunning 的輪次)// 每 (param.nExps * 	param.AggreIter) 次，計算 mean 跟 std (param.AggreIter 見 main_client.cpp) 
    Input Parameters
        ----------
        Eva_num     : array ; 1 - by - param.nExps * 	param.AggreIter(10 * 5)    ; //儲存每輪局部更新完獲得的各項評估指標，Eva_oneItertime(包含 train 和 test)
    Output Parameters
        ----------
        mean        : double;  平均值
        std         : double;  標準差

*/
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <ctime>
#include <numeric>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "meanstd_client.h"
using namespace Eigen;
using namespace std;


std::tuple<double, double>  meanstd_client(vector<double> Eva_num)
{
    // 計算 mean
    double mean = std::accumulate(Eva_num.begin(), Eva_num.end(), 0.0) / Eva_num.size();
    // 計算 std
    double accum = 0.0;
    std::for_each(Eva_num.begin(), Eva_num.end(), [&](const double d) {
        accum += (d - mean) * (d - mean); });
    double stdev = sqrt(accum / Eva_num.size());
    return std::make_tuple(mean, stdev);
}

