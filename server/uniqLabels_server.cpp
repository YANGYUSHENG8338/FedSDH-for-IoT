/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
去除 vector 陣列裡重複的數值，並將數值由小排到大，目的是算 Label 的種類有幾個
Input Parameters
        ----------
        Eigen_Label                   : string&             ; 載入控制指令
 Output Parameters
        ----------
        Label_uni                     : array  ; 1 - by - C ; 訓練資料矩陣
        ip                            : array  ; 1 - by - C ; 微調資料矩陣
*/
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "uniqLabels_server.h"
using namespace std;
using namespace Eigen;
//### 排版
tuple<vector<double>, vector<double>::iterator > uniqLabels_server(VectorXd Eigen_Label) 
{
    vector<double>           Label;
    vector<double>::iterator ip;
    Label.resize(Eigen_Label.size());
    VectorXd::Map(&Label[0], Eigen_Label.size()) = Eigen_Label;
    vector<double>           Label_uni(Label.begin(), Label.end());

    // Sorting the array 
    sort(Label_uni.begin(), Label_uni.end());
    // 找出 Label 類別數
    ip = unique(Label_uni.begin(), Label_uni.begin() + Eigen_Label.rows());
    Label_uni.resize(std::distance(Label_uni.begin(), ip));
    return std::make_tuple(Label_uni, ip);

}