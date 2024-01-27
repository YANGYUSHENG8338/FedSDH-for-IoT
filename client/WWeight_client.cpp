/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)


@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�
***�Ƶ{���γ~***
��s�C�ӽ����Ĥ@�^������s�v�� W �ϥΦ��� ; �ĤG�^�H��ϥ� WWeightRRC_client.cpp

�ܼƦW�� �}�Y�� D => DOUBLE ; I => INT ;
    Input Parameters
        ----------
        *DWWeightData       : array*  ; Ntran - by - C ;   // �V�m��Ưx�}( B )
        *WWeightLabels      : array*  ; C     - by - N ;   // ���ո�Ưx�}( Y ) (�W���s�X)
        lambda              : double  ;                    // SDH �����h�ưѼ�
        SwitchMethod        : string  ;                    // ��ܦb���Ӹ˸m�W�i���v����s(NANO , RASP)
    Output Parameters
        ----------
        *DWWeight           : array*  ; H     - by - L ;   // ������s�v�� ( WWeight )

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
    Matrix<double, Dynamic, Dynamic>* DWWeight      = new Matrix<double, Dynamic, Dynamic>();                 // �ŧi W ������s�v��
    Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                 // �p���v�x�}�ɪ��L��x�} 1
    Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                 // �p���v�x�}�ɪ��L��x�} 2
    MatrixXd* DWWeightOneHotLabels                  = new MatrixXd(IWWeightOneHotLabels->cast<double>());     // �N�쥻 int ���A Label (�W���s�X)�ഫ�� double ���A
    // prjection matrix computing

    if ((SwitchMethod.compare("PROX_SDH") == 0) || (SwitchMethod.compare("AVG_SDH") == 0))                    // SDH �����p�����ǲ߼ҫ��ϥ�
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
    else if ((SwitchMethod.compare("PROX_FSDH") == 0) || (SwitchMethod.compare("AVG_FSDH") == 0))            // FSDH �����p�����ǲ߼ҫ��ϥ�
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


    // �����ʺA���t�O����
    delete midProjMtx ;
    delete finalProjMtx ;
    return std::make_tuple(DWWeight);
}