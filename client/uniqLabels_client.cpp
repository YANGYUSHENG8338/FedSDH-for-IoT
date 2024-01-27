/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.1
Log: 0.0.1 Initial
Input Parameters
        ----------
***副程式用途***
去除 vector 陣列裡重複的數值，並將數值由小排到大，目的是算 Label 的種類有幾個
Input Parameters
        ----------
        Eigen_Label              : string&             ; 載入控制指令
 Output Parameters
        ----------
        Label_uni                : array  ; 1 - by - C ; 訓練資料矩陣
        ip                       : array  ; 1 - by - C ; 微調資料矩陣
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
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "uniqLabels_client.h"
using namespace std;
using namespace Eigen;
using namespace cv;

tuple<vector<int>, vector<int>::iterator > uniqLabels_client(MatrixXi* MatrixXi_Label)
{ 
    VectorXi              VectorXi_Label            = Map<VectorXi>(MatrixXi_Label->data(), MatrixXi_Label->rows() * MatrixXi_Label->cols());
    vector<int>           Label;
    Label.resize(VectorXi_Label.size());
    VectorXi::Map(&Label[0], VectorXi_Label.size()) = VectorXi_Label;
    vector<int>           Label_uni(Label.begin(), Label.end());
    vector<int>::iterator ip;

    // Sorting the array 
    sort(Label_uni.begin(), Label_uni.end());
    // 找出 Label 類別數
    ip = unique(Label_uni.begin(), Label_uni.begin() + VectorXi_Label.rows());
    Label_uni.resize(std::distance(Label_uni.begin(), ip));
    return std::make_tuple(Label_uni, ip);
}