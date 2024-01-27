/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
更新每個輪次第一回局部更新權重 P 使用此檔 ; 第二回以後使用 PWeightRRC_client.cpp


變數名稱 開頭為 D => DOUBLE ; I => INT ;
    Input Parameters
		----------            
		*DPWeightData         : array*  ; Ntran - by - L ; // 訓練資料矩陣( Phi(X) )
		*IBWeight             : array*  ; Ntran - by - C ; // 測試資料矩陣( B )
		lambda                : double  ;                  // SDH 的正則化參數
	Output Parameters
		----------
		*DPWeight             : array*  ; H     - by - L ; // 局部更新權重 ( PWeight )


*/
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "SDH_client.h"
#include "PWeight_client.h"
#include "uniqLabels_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;


std::tuple<MatrixXd*> PWeight_client(MatrixXd* DPWeightData, MatrixXi* IBWeight, double lambda, int LabelCount)
{
    Matrix<double, Dynamic, Dynamic>* DPWeight      = new Matrix<double, Dynamic, Dynamic>();                // 宣告 P 局部更新權重      變數名稱 開頭為 D => DOUBLE ; I => INT ;
    Matrix<double, Dynamic, Dynamic>* DBWeight      = new Matrix<double, Dynamic, Dynamic>();                // 宣告 B 局部更新權重      
    Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                // 計算投影矩陣時的過渡矩陣 1
    Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                // 計算投影矩陣時的過渡矩陣 2

   
    //ONE_HOT_ENCODING
    auto        uniqLabelsData                      = uniqLabels_client(IBWeight);
    vector<int> uniqLabelList                       = std::get<0>(uniqLabelsData);
    MatrixXi    IBWeightOneHot                      = MatrixXi::Zero(IBWeight->rows(), LabelCount);
    // 由於 Matlab 的起始值不同，所以需扣掉 1
    if (IBWeight->cols() == 1)
    {
        for (int i = 0; i < IBWeight->rows(); i++)
        {
            IBWeightOneHot(i, (*IBWeight)(i, 0) - 1) = 1;
            *DBWeight                                          = IBWeightOneHot.cast<double>();               // 將原本 int 型態 B 局部更新權重 轉換成 double 型態
        }
    }
    else
    {
        *DBWeight                                              = IBWeight->cast<double>();                     // 將原本 int 型態 B 局部更新權重 轉換成 double 型態
    }


    //prjection matrix computing
    // 由於此處所有聯邦式學習模型產生初始 P 局部更新權重都相同，因此不需選擇模型
    if (DPWeightData->rows() < DPWeightData->cols())
    {
        if (IBWeight->rows() < IBWeight->cols())
        {
            *midProjMtx  = ((*DPWeightData) * (*DPWeightData).transpose() + lambda * MatrixXd::Identity((*DPWeightData).rows(), (*DPWeightData).rows()) + MatrixXd::Zero((*DPWeightData).rows(), (*DPWeightData).rows()));
        }
        else
        {
            *midProjMtx  = ((*DPWeightData) * (*DPWeightData).transpose() + lambda * MatrixXd::Identity((*DPWeightData).rows(), (*DPWeightData).rows()) + MatrixXd::Zero((*DPWeightData).rows(), (*DPWeightData).rows()));
        }
        *finalProjMtx    = (*DPWeightData).transpose() * midProjMtx ->inverse();
    }
    else
    {
        *midProjMtx      = ((*DPWeightData).transpose() * (*DPWeightData) + lambda * MatrixXd::Identity((*DPWeightData).cols(), (*DPWeightData).cols()) + MatrixXd::Zero((*DPWeightData).cols(), (*DPWeightData).cols()));
        *finalProjMtx    = midProjMtx ->ldlt().solve((*DPWeightData).transpose());
    }
    *DPWeight            = (*finalProjMtx ) * (*DBWeight);

    // 消除動態分配記憶體
    delete midProjMtx ;
    delete finalProjMtx ;

    return std::make_tuple(DPWeight);
}

