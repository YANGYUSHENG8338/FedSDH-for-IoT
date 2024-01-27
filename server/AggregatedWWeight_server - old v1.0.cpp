/*
@author:     Yu-Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo-Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    
Log: 0.0.1 Initial


***�Ƶ{���γ~***
�E�X CLIENT �� WWeight ������s�v��
Input Parameters
        ----------
        RENEW_RASP_labelList        : array  ; C_label1     - by - 1 ; RASP �ݼ������O�x�}
        RENEW_NANO_labelList        : array  ; C_label2     - by - 1 ; NANO �ݼ������O�x�}
        RENEW_RASP_WWeight          : array  ; C            - by - L ; RASP �ݦ^�� SERVER ������s�v�� ( WWeight )
        RENEW_NANO_WWeight          : array  ; C            - by - L ; NANO �ݦ^�� SERVER ������s�v�� ( WWeight )
        LabelCount                  : int    : C                       ����ƶ��Ҧ����O��
        paramL                      : int    :                         �s�X����
        raspDataRatio                                                  ���t��Ƶ� RASP �����
        nanoDataRatio                                                  ���t��Ƶ� NANO �����
 Output Parameters
        ----------
        AGGREGATED_RASP_WWeight     : array  ; commonValues - by - 1 ; SERVER �ݻE�X���Ǩ� RASP �ݪ�������s�v�� ( WWeight )
        AGGREGATED_NANO_WWeight     : array  ; commonValues - by - 1 ; SERVER �ݻE�X���Ǩ� NANO �ݪ�������s�v�� ( WWeight )
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
    MatrixXd commonLabels = std::get<0>(compareLableClass_server(RENEW_RASP_labelList, RENEW_NANO_labelList));    // ���X��� client �ݭ��Ƽ������O�x�}
    VectorXd VRASP_labelList = RENEW_RASP_labelList.col(0);                                                       // �N RASP_labelList �ഫ���V�q VectorXd �A�]���e���h V
    VectorXd VNANO_labelList = RENEW_NANO_labelList.col(0);                                                       // �N NANO_labelList �ഫ���V�q VectorXd �A�]���e���h V
    VectorXd RASP_NonlabelList = VectorXd::Zero(LabelCount - RENEW_RASP_labelList.size());                        // �s�x RASP �ݨS����o���V�q
    VectorXd NANO_NonlabelList = VectorXd::Zero(LabelCount - RENEW_NANO_labelList.size());                        // �s�x NANO �ݨS����o���V�q

    VectorXd AllLabel(LabelCount);

    for (int LabelIdx = 0; LabelIdx < LabelCount; LabelIdx++) {
        AllLabel(LabelIdx) = LabelIdx + 1;
    }

    // RASP �ݧ�X���� Label ������ۤv Label ���������B��
    auto RASPit = std::set_difference(AllLabel.data(), AllLabel.data() + AllLabel.size(), VRASP_labelList.data(), VRASP_labelList.data() + VRASP_labelList.size(), RASP_NonlabelList.data());
    RASP_NonlabelList.conservativeResize(std::distance(RASP_NonlabelList.data(), RASPit));
    // NANO �ݧ�X���� Label ������ۤv Label ���������B��
    auto NANOit = std::set_difference(AllLabel.data(), AllLabel.data() + AllLabel.size(), VNANO_labelList.data(), VNANO_labelList.data() + VNANO_labelList.size(), NANO_NonlabelList.data());
    NANO_NonlabelList.conservativeResize(std::distance(NANO_NonlabelList.data(), NANOit));



    MatrixXd AGGREGATED_WWeight = MatrixXd::Zero(LabelCount, paramL);   // �ЫئX�� WWeight �x�};
    //   AGGREGATED_WWeight �x�}�X�� RASP �� WWeight �x�}
    for (int RASP_labelListIdx = 0; RASP_labelListIdx < RENEW_RASP_labelList.rows(); RASP_labelListIdx++)
    {
        AGGREGATED_WWeight.row(RENEW_RASP_labelList(RASP_labelListIdx, 0) - 1) += RENEW_RASP_WWeight.row(RASP_labelListIdx);
    }
    //  AGGREGATED_WWeight �x�}�X�� NANO �� WWeight �x�}
    for (int NANO_labelListIdx = 0; NANO_labelListIdx < RENEW_NANO_labelList.rows(); NANO_labelListIdx++)
    {
        AGGREGATED_WWeight.row(RENEW_NANO_labelList(NANO_labelListIdx, 0) - 1) += RENEW_NANO_WWeight.row(NANO_labelListIdx);
    }
    // ��h RASP �M NANO ���ƭp�⪺ WWeight �x�}
    for (int commonLabelsIdx = 0; commonLabelsIdx < commonLabels.size(); commonLabelsIdx++)
    {
        AGGREGATED_WWeight.row(RENEW_RASP_labelList(commonLabelsIdx, 0) - 1) -= RENEW_RASP_WWeight.row(commonLabelsIdx) * (1 - raspDataRatio);
        AGGREGATED_WWeight.row(RENEW_NANO_labelList(commonLabelsIdx, 0) - 1) -= RENEW_NANO_WWeight.row(commonLabelsIdx) * (1 - nanoDataRatio);
    }

    MatrixXd AGGREGATED_RASP_WWeight = MatrixXd::Zero(LabelCount, paramL);   // �Ы� ���o�� RASP �� WWeight �x�}
    MatrixXd AGGREGATED_NANO_WWeight = MatrixXd::Zero(LabelCount, paramL);   // �Ы� ���o�� NANO �� WWeight �x�}

    for (int RASP_labelListIdx = 0; RASP_labelListIdx < RENEW_RASP_labelList.rows(); RASP_labelListIdx++)           // ���X ���o�� RASP �� WWeight �x�}
    {
        AGGREGATED_RASP_WWeight.row(RENEW_RASP_labelList(RASP_labelListIdx, 0) - 1) += AGGREGATED_WWeight.row(RASP_labelListIdx);
    }
    for (int NANO_labelListIdx = 0; NANO_labelListIdx < RENEW_NANO_labelList.rows(); NANO_labelListIdx++)           // ���X ���o�� NANO �� WWeight �x�}
    {
        AGGREGATED_NANO_WWeight.row(RENEW_NANO_labelList(NANO_labelListIdx, 0) - 1) += AGGREGATED_WWeight.row(NANO_labelListIdx);
    }


    for (int RASP_NonlabelListIdx = 0; RASP_NonlabelListIdx < RASP_NonlabelList.size(); RASP_NonlabelListIdx++)     // �N�S�� Label�� WWeight �x�}���� row �]�� NaN
    {
        AGGREGATED_RASP_WWeight.row(RASP_NonlabelList(RASP_NonlabelListIdx) - 1).setConstant(std::numeric_limits<double>::quiet_NaN());
    }
    for (int NANO_NonlabelListIdx = 0; NANO_NonlabelListIdx < NANO_NonlabelList.size(); NANO_NonlabelListIdx++)      // �N�S�� Label�� WWeight �x�}���� row �]�� NaN
    {
        AGGREGATED_NANO_WWeight.row(NANO_NonlabelList(NANO_NonlabelListIdx) - 1).setConstant(std::numeric_limits<double>::quiet_NaN());
    }






    return std::make_tuple(AGGREGATED_RASP_WWeight, AGGREGATED_NANO_WWeight);

}