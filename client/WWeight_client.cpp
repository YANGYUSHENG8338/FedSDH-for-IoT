/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)


@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式
***副程式用途***
更新每個輪次第一回局部更新權重 W 使用此檔 ; 第二回以後使用 WWeightRRC_client.cpp

變數名稱 開頭為 D => DOUBLE ; I => INT ;
    Input Parameters
        ----------
        *DWWeightData       : array*  ; Ntran - by - C ;   // 訓練資料矩陣( B )
        *WWeightLabels      : array*  ; C     - by - N ;   // 測試資料矩陣( Y ) (獨熱編碼)
        lambda              : double  ;                    // SDH 的正則化參數
        SwitchMethod        : string  ;                    // 選擇在哪個裝置上進行權重更新(NANO , RASP)
    Output Parameters
        ----------
        *DWWeight           : array*  ; H     - by - L ;   // 局部更新權重 ( WWeight )

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
#include "WWeight_client.h"
#include "uniqLabels_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


std::tuple<MatrixXd*> WWeight_client(MatrixXd* DWWeightData, MatrixXi* IWWeightOneHotLabels, double lambda, string SwitchMethod)
{
    Matrix<double, Dynamic, Dynamic>* DWWeight      = new Matrix<double, Dynamic, Dynamic>();                 // 宣告 W 局部更新權重
    Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                 // 計算投影矩陣時的過渡矩陣 1
    Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                 // 計算投影矩陣時的過渡矩陣 2
    MatrixXd* DWWeightOneHotLabels                  = new MatrixXd(IWWeightOneHotLabels->cast<double>());     // 將原本 int 型態 Label (獨熱編碼)轉換成 double 型態
    // prjection matrix computing

    if ((SwitchMethod.compare("PROX_SDH") == 0) || (SwitchMethod.compare("AVG_SDH") == 0))                    // SDH 相關聯邦式學習模型使用
    {
        if (DWWeightData->rows() < DWWeightData->cols())
        {
            if (IWWeightOneHotLabels->rows() < IWWeightOneHotLabels->cols())
            {
                *midProjMtx = ((*DWWeightData) * (*DWWeightData).transpose() + lambda * MatrixXd::Identity((*DWWeightData).rows(), (*DWWeightData).rows()) + MatrixXd::Zero((*DWWeightData).rows(), (*DWWeightData).rows()));
            }
            else
            {
                *midProjMtx = ((*DWWeightData) * (*DWWeightData).transpose() + lambda * MatrixXd::Identity((*DWWeightData).rows(), (*DWWeightData).rows()) + MatrixXd::Zero((*DWWeightData).rows(), (*DWWeightData).rows()));
            }
            *finalProjMtx    = (*DWWeightData).transpose() * midProjMtx ->inverse();
        }
        else
        {
            *midProjMtx      = ((*DWWeightData).transpose() * (*DWWeightData) + lambda * MatrixXd::Identity((*DWWeightData).cols(), (*DWWeightData).cols()) + MatrixXd::Zero((*DWWeightData).cols(), (*DWWeightData).cols()));
            *finalProjMtx    = midProjMtx ->ldlt().solve((*DWWeightData).transpose());
        }
        *DWWeight            = (*finalProjMtx ) * (*DWWeightOneHotLabels);
    }
    else if ((SwitchMethod.compare("PROX_FSDH") == 0) || (SwitchMethod.compare("AVG_FSDH") == 0))            // FSDH 相關聯邦式學習模型使用
    {
        if ((*DWWeightOneHotLabels).rows() < (*DWWeightOneHotLabels).cols())
        {
            if (DWWeightData->rows() < DWWeightData->cols())
            {
                *midProjMtx  = ((*DWWeightOneHotLabels) * (*DWWeightOneHotLabels).transpose() + lambda * MatrixXd::Identity(DWWeightOneHotLabels->rows(), DWWeightOneHotLabels->rows()) + MatrixXd::Zero(DWWeightOneHotLabels->rows(), DWWeightOneHotLabels->rows()));
            }
            else
            {
                *midProjMtx  = ((*DWWeightOneHotLabels) * (*DWWeightOneHotLabels).transpose() + lambda * MatrixXd::Identity(DWWeightOneHotLabels->rows(), DWWeightOneHotLabels->rows()) + MatrixXd::Zero(DWWeightOneHotLabels->rows(), DWWeightOneHotLabels->rows()));
            }
            *finalProjMtx    = (*DWWeightOneHotLabels).transpose() * midProjMtx ->inverse();
        }
        else
        {
            int i = 0;
            *midProjMtx      = ((*DWWeightOneHotLabels).transpose() * (*DWWeightOneHotLabels) + lambda * MatrixXd::Identity((*DWWeightOneHotLabels).cols(), (*DWWeightOneHotLabels).cols()) + MatrixXd::Zero((*DWWeightOneHotLabels).cols(), (*DWWeightOneHotLabels).cols()));
            *finalProjMtx    = midProjMtx ->ldlt().solve((*DWWeightOneHotLabels).transpose());
        }

        *DWWeight            = *finalProjMtx  * (*DWWeightData);
    }


    // 消除動態分配記憶體
    delete midProjMtx ;
    delete finalProjMtx ;
    return std::make_tuple(DWWeight);
}