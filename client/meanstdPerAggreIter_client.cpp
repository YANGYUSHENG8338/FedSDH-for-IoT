/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1   Initial
Log: 0.0.2   �令���ЧΦ�

***�Ƶ{���γ~***
�p�� Testing ����������(���]�t Tunning ���^�ơA�u�p��̫�@���ϥ� TestData �U�����G) // �C param.AggreIter ���A�p�� mean �� std (param.AggreIter �� main_client.cpp) 
    Input Parameters
        ----------
        Eva_num     : array ; 1 - by - param.LocalIteration(10); //�x�s�C��������s����o���U���������СAEva_OneAggreTime(�]�t train �M test)
    Output Parameters
        ----------
        mean        : double;  ������
		std         : double;  �зǮt

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
	// �p�� mean
	double mean = std::accumulate(EvaNuminterval.begin(), EvaNuminterval.end(), 0.0) / EvaNuminterval.size();
	// �p�� std
	double accum = 0.0;
	std::for_each(EvaNuminterval.begin(), EvaNuminterval.end(), [&](const double d) {
		accum += (d - mean) * (d - mean); });
	double stdev = sqrt(accum / EvaNuminterval.size());
	return std::make_tuple(mean, stdev);
}

