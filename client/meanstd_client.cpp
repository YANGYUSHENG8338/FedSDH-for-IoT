/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1   Initial
Log: 0.0.2   �令���ЧΦ�

***�Ƶ{���γ~***
// �p��C�@���E�X��������������(�]�t Tunning ������)// �C (param.nExps * 	param.AggreIter) ���A�p�� mean �� std (param.AggreIter �� main_client.cpp) 
    Input Parameters
        ----------
        Eva_num     : array ; 1 - by - param.nExps * 	param.AggreIter(10 * 5)    ; //�x�s�C��������s����o���U���������СAEva_oneItertime(�]�t train �M test)
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
#include <Eigen/Core>
#include <Eigen/Dense>
#include "meanstd_client.h"
using namespace Eigen;
using namespace std;


std::tuple<double, double>  meanstd_client(vector<double> Eva_num)
{
    // �p�� mean
    double mean = std::accumulate(Eva_num.begin(), Eva_num.end(), 0.0) / Eva_num.size();
    // �p�� std
    double accum = 0.0;
    std::for_each(Eva_num.begin(), Eva_num.end(), [&](const double d) {
        accum += (d - mean) * (d - mean); });
    double stdev = sqrt(accum / Eva_num.size());
    return std::make_tuple(mean, stdev);
}

