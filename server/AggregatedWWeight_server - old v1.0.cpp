/*
@author:     Yu-Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo-Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    
Log: 0.0.1 Initial


***副程式用途***
聚合 CLIENT 端 WWeight 局部更新權重
Input Parameters
        ----------
        RENEW_RASP_labelList        : array  ; C_label1     - by - 1 ; RASP 端標籤類別矩陣
        RENEW_NANO_labelList        : array  ; C_label2     - by - 1 ; NANO 端標籤類別矩陣
        RENEW_RASP_WWeight          : array  ; C            - by - L ; RASP 端回傳 SERVER 局部更新權重 ( WWeight )
        RENEW_NANO_WWeight          : array  ; C            - by - L ; NANO 端回傳 SERVER 局部更新權重 ( WWeight )
        LabelCount                  : int    : C                       此資料集所有類別數
        paramL                      : int    :                         編碼長度
        raspDataRatio                                                  分配資料給 RASP 之比例
        nanoDataRatio                                                  分配資料給 NANO 之比例
 Output Parameters
        ----------
        AGGREGATED_RASP_WWeight     : array  ; commonValues - by - 1 ; SERVER 端聚合完傳到 RASP 端的全局更新權重 ( WWeight )
        AGGREGATED_NANO_WWeight     : array  ; commonValues - by - 1 ; SERVER 端聚合完傳到 NANO 端的全局更新權重 ( WWeight )
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <limits>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "compareLableClass_server.h"
#include "AggregatedWWeight_server.h"
using namespace std;
using namespace Eigen;

tuple< MatrixXd, MatrixXd> AggregatedWWeight_server(MatrixXd RENEW_RASP_labelList, MatrixXd RENEW_NANO_labelList, MatrixXd RENEW_RASP_WWeight, MatrixXd RENEW_NANO_WWeight, int LabelCount,int paramL,double raspDataRatio,double nanoDataRatio)
{
    MatrixXd commonLabels = std::get<0>(compareLableClass_server(RENEW_RASP_labelList, RENEW_NANO_labelList));    // 取出兩個 client 端重複標籤類別矩陣
    VectorXd VRASP_labelList = RENEW_RASP_labelList.col(0);                                                       // 將 RASP_labelList 轉換為向量 VectorXd ，因此前面多 V
    VectorXd VNANO_labelList = RENEW_NANO_labelList.col(0);                                                       // 將 NANO_labelList 轉換為向量 VectorXd ，因此前面多 V
    VectorXd RASP_NonlabelList = VectorXd::Zero(LabelCount - RENEW_RASP_labelList.size());                        // 存儲 RASP 端沒有獲得的向量
    VectorXd NANO_NonlabelList = VectorXd::Zero(LabelCount - RENEW_NANO_labelList.size());                        // 存儲 NANO 端沒有獲得的向量

    VectorXd AllLabel(LabelCount);

    for (int LabelIdx = 0; LabelIdx < LabelCount; LabelIdx++) {
        AllLabel(LabelIdx) = LabelIdx + 1;
    }

    // RASP 端找出全部 Label 種類減掉自己 Label 的種類的運算
    auto RASPit = std::set_difference(AllLabel.data(), AllLabel.data() + AllLabel.size(), VRASP_labelList.data(), VRASP_labelList.data() + VRASP_labelList.size(), RASP_NonlabelList.data());
    RASP_NonlabelList.conservativeResize(std::distance(RASP_NonlabelList.data(), RASPit));
    // NANO 端找出全部 Label 種類減掉自己 Label 的種類的運算
    auto NANOit = std::set_difference(AllLabel.data(), AllLabel.data() + AllLabel.size(), VNANO_labelList.data(), VNANO_labelList.data() + VNANO_labelList.size(), NANO_NonlabelList.data());
    NANO_NonlabelList.conservativeResize(std::distance(NANO_NonlabelList.data(), NANOit));



    MatrixXd AGGREGATED_WWeight = MatrixXd::Zero(LabelCount, paramL);   // 創建合併 WWeight 矩陣;
    //   AGGREGATED_WWeight 矩陣合併 RASP 的 WWeight 矩陣
    for (int RASP_labelListIdx = 0; RASP_labelListIdx < RENEW_RASP_labelList.rows(); RASP_labelListIdx++)
    {
        AGGREGATED_WWeight.row(RENEW_RASP_labelList(RASP_labelListIdx, 0) - 1) += RENEW_RASP_WWeight.row(RASP_labelListIdx);
    }
    //  AGGREGATED_WWeight 矩陣合併 NANO 的 WWeight 矩陣
    for (int NANO_labelListIdx = 0; NANO_labelListIdx < RENEW_NANO_labelList.rows(); NANO_labelListIdx++)
    {
        AGGREGATED_WWeight.row(RENEW_NANO_labelList(NANO_labelListIdx, 0) - 1) += RENEW_NANO_WWeight.row(NANO_labelListIdx);
    }
    // 減去 RASP 和 NANO 重複計算的 WWeight 矩陣
    for (int commonLabelsIdx = 0; commonLabelsIdx < commonLabels.size(); commonLabelsIdx++)
    {
        AGGREGATED_WWeight.row(RENEW_RASP_labelList(commonLabelsIdx, 0) - 1) -= RENEW_RASP_WWeight.row(commonLabelsIdx) * (1 - raspDataRatio);
        AGGREGATED_WWeight.row(RENEW_NANO_labelList(commonLabelsIdx, 0) - 1) -= RENEW_NANO_WWeight.row(commonLabelsIdx) * (1 - nanoDataRatio);
    }

    MatrixXd AGGREGATED_RASP_WWeight = MatrixXd::Zero(LabelCount, paramL);   // 創建 分發給 RASP 的 WWeight 矩陣
    MatrixXd AGGREGATED_NANO_WWeight = MatrixXd::Zero(LabelCount, paramL);   // 創建 分發給 NANO 的 WWeight 矩陣

    for (int RASP_labelListIdx = 0; RASP_labelListIdx < RENEW_RASP_labelList.rows(); RASP_labelListIdx++)           // 取出 分發給 RASP 的 WWeight 矩陣
    {
        AGGREGATED_RASP_WWeight.row(RENEW_RASP_labelList(RASP_labelListIdx, 0) - 1) += AGGREGATED_WWeight.row(RASP_labelListIdx);
    }
    for (int NANO_labelListIdx = 0; NANO_labelListIdx < RENEW_NANO_labelList.rows(); NANO_labelListIdx++)           // 取出 分發給 NANO 的 WWeight 矩陣
    {
        AGGREGATED_NANO_WWeight.row(RENEW_NANO_labelList(NANO_labelListIdx, 0) - 1) += AGGREGATED_WWeight.row(NANO_labelListIdx);
    }


    for (int RASP_NonlabelListIdx = 0; RASP_NonlabelListIdx < RASP_NonlabelList.size(); RASP_NonlabelListIdx++)     // 將沒有 Label的 WWeight 矩陣中的 row 設為 NaN
    {
        AGGREGATED_RASP_WWeight.row(RASP_NonlabelList(RASP_NonlabelListIdx) - 1).setConstant(std::numeric_limits<double>::quiet_NaN());
    }
    for (int NANO_NonlabelListIdx = 0; NANO_NonlabelListIdx < NANO_NonlabelList.size(); NANO_NonlabelListIdx++)      // 將沒有 Label的 WWeight 矩陣中的 row 設為 NaN
    {
        AGGREGATED_NANO_WWeight.row(NANO_NonlabelList(NANO_NonlabelListIdx) - 1).setConstant(std::numeric_limits<double>::quiet_NaN());
    }






    return std::make_tuple(AGGREGATED_RASP_WWeight, AGGREGATED_NANO_WWeight);

}