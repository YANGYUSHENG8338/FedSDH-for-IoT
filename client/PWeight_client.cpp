/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
��s�C�ӽ����Ĥ@�^������s�v�� P �ϥΦ��� ; �ĤG�^�H��ϥ� PWeightRRC_client.cpp


�ܼƦW�� �}�Y�� D => DOUBLE ; I => INT ;
    Input Parameters
		----------            
		*DPWeightData         : array*  ; Ntran - by - L ; // �V�m��Ưx�}( Phi(X) )
		*IBWeight             : array*  ; Ntran - by - C ; // ���ո�Ưx�}( B )
		lambda                : double  ;                  // SDH �����h�ưѼ�
	Output Parameters
		----------
		*DPWeight             : array*  ; H     - by - L ; // ������s�v�� ( PWeight )


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
    Matrix<double, Dynamic, Dynamic>* DPWeight      = new Matrix<double, Dynamic, Dynamic>();                // �ŧi P ������s�v��      �ܼƦW�� �}�Y�� D => DOUBLE ; I => INT ;
    Matrix<double, Dynamic, Dynamic>* DBWeight      = new Matrix<double, Dynamic, Dynamic>();                // �ŧi B ������s�v��      
    Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                // �p���v�x�}�ɪ��L��x�} 1
    Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                // �p���v�x�}�ɪ��L��x�} 2

   
    //ONE_HOT_ENCODING
    auto        uniqLabelsData                      = uniqLabels_client(IBWeight);
    vector<int> uniqLabelList                       = std::get<0>(uniqLabelsData);
    MatrixXi    IBWeightOneHot                      = MatrixXi::Zero(IBWeight->rows(), LabelCount);
    // �ѩ� Matlab ���_�l�Ȥ��P�A�ҥH�ݦ��� 1
    if (IBWeight->cols() == 1)
    {
        for (int i = 0; i < IBWeight->rows(); i++)
        {
            IBWeightOneHot(i, (*IBWeight)(i, 0) - 1) = 1;
            *DBWeight                                          = IBWeightOneHot.cast<double>();               // �N�쥻 int ���A B ������s�v�� �ഫ�� double ���A
        }
    }
    else
    {
        *DBWeight                                              = IBWeight->cast<double>();                     // �N�쥻 int ���A B ������s�v�� �ഫ�� double ���A
    }


    //prjection matrix computing
    // �ѩ󦹳B�Ҧ��p�����ǲ߼ҫ����ͪ�l P ������s�v�����ۦP�A�]�����ݿ�ܼҫ�
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

    // �����ʺA���t�O����
    delete midProjMtx ;
    delete finalProjMtx ;

    return std::make_tuple(DPWeight);
}

