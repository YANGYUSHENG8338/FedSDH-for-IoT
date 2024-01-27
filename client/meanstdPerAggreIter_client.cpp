/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1   Initial
Log: 0.0.2   改成指標形式

***副程式用途***
計算 Testing 的評估指標(不包含 Tunning 的回數，只計算最後一輪使用 TestData 下的結果) // 每 param.AggreIter 次，計算 mean 跟 std (param.AggreIter 見 main_client.cpp) 
    Input Parameters
        ----------
        Eva_num     : array ; 1 - by - param.LocalIteration(10); //儲存每輪局部更新完獲得的各項評估指標，Eva_OneAggreTime(包含 train 和 test)
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
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
#include "meanstdPerAggreIter_client.h"
using namespace Eigen;
using namespace std;


std::tuple<double, double>  meanstdPerAggreIter_client(vector<double> Eva_num, int aggreItr) // @@@ meanstd_10_client => meanstdPerAggreIter_client
{
	vector<double> EvaNuminterval;
	int count = aggreItr - 1;
	for (int i = 0; i < Eva_num.size() / aggreItr; i++)
	{
		EvaNuminterval.push_back(Eva_num[count]);
		count = count + aggreItr;
	}
	// 計算 mean
	double mean = std::accumulate(EvaNuminterval.begin(), EvaNuminterval.end(), 0.0) / EvaNuminterval.size();
	// 計算 std
	double accum = 0.0;
	std::for_each(EvaNuminterval.begin(), EvaNuminterval.end(), [&](const double d) {
		accum += (d - mean) * (d - mean); });
	double stdev = sqrt(accum / EvaNuminterval.size());
	return std::make_tuple(mean, stdev);
}

