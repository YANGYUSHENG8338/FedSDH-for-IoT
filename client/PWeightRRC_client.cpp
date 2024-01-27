/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
��s�C�ӽ����Ĥ@�^������s�v�� P �ϥ� PWeight_client.cpp ; �ĤG�^�H��ϥΦ���

�ܼƦW�� �}�Y�� D => DOUBLE ; I => INT ;
	Input Parameters
		----------
		*DPWeightData     : array*  ; Ntran - by - L ; // �V�m��Ưx�}( Phi(X) )
		*IBWeight         : array*  ; Ntran - by - C ; // ���ո�Ưx�}( B )
		lambda            : double  ;                  // SDH �����h�ưѼ�
		nu                : double  ;                  // �g�@�Ѽ�
		mu                : double  ;                  // FedProx �����h�ưѼ�

	Output Parameters
		----------
		*DPWeight         : array*  ; H     - by - L ; // �����v�� ( PWeight )


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
#include "LoadData_client.h"
#include "SDH_client.h"
#include "PWeightRRC_client.h"
#include "uniqLabels_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;


std::tuple<MatrixXd*> PWeightRRC_client(MatrixXd* DPWeightData, MatrixXi* IBWeight, double nu, double mu, string str_EXP, string str_ITR, string SwitchMethod, string LoadDataPath, int LabelCount)
{

	Matrix<double, Dynamic, Dynamic>* DPWeight      = new Matrix<double, Dynamic, Dynamic>();                 // �ŧi P ������s�v��         �ܼƦW�� �}�Y�� D => DOUBLE ; I => INT ;
	Matrix<double, Dynamic, Dynamic>* DBWeight      = new Matrix<double, Dynamic, Dynamic>();                 // �ŧi B ������s�v��
	Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                 // �p���v�x�}�ɪ��L��x�} 1
	Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                 // �p���v�x�}�ɪ��L��x�} 2



	//Load SERVER AGGREGATED PWeight
    auto     Assign_PWeight                         = LoadData_client(LoadDataPath + str_EXP + "_" + str_ITR + INIT_CLIENT_PWeightMtxTxt);
	MatrixXd *SERVER_INIT_PWeight                   = new MatrixXd(std::get<0>(Assign_PWeight));

	//��ܨϥΦ�ؼҫ�
	if (SwitchMethod.compare("PROX_SDH") == 0)                                                                // PROX_SDH �p�����ǲ߼ҫ��ϥ�
	{
		//ONE_HOT_ENCODING
		auto        uniqLabelsData                  = uniqLabels_client(IBWeight);
		vector<int> uniqLabelList                   = std::get<0>(uniqLabelsData);
		MatrixXi    IBWeightOneHot                  = MatrixXi::Zero(IBWeight->rows(), LabelCount);
		// �ѩ� Matlab ���_�l�Ȥ��P�A�ҥH�ݦ��� 1 
		if (IBWeight->cols() == 1)
		{
			for (int i = 0; i < IBWeight->rows(); i++)
			{
				IBWeightOneHot(i, (*IBWeight)(i, 0) - 1) = 1;
				*DBWeight                                          = IBWeightOneHot.cast<double>();           // �N�쥻 int ���A B ������s�v�� �ഫ�� double ���A
			}
		}
		else
		{
			*DBWeight                                              = IBWeight->cast<double>();                // �N�쥻 int ���A B ������s�v�� �ഫ�� double ���A
		}


		//prjection matrix computing
		if (DPWeightData->rows() < DPWeightData->cols())
		{
			if (IBWeight->rows() < IBWeight->cols())
			{
				*midProjMtx  = ((*DPWeightData) * (*DPWeightData).transpose());
			}
			else
			{
				*midProjMtx  = ((*DPWeightData) * (*DPWeightData).transpose());
			}
			*finalProjMtx    = (*DPWeightData).transpose() * (*midProjMtx ).inverse();
			*DPWeight        = (2 / (2 * nu + mu)) * (((nu * (*DBWeight) + (mu / 2) * ((*DPWeightData) * (*SERVER_INIT_PWeight)))) * (*finalProjMtx ));
		}
		else
		{
			*midProjMtx      = ((*DPWeightData).transpose() * (*DPWeightData));
			*DPWeight        = (2 / (2 * nu + mu)) * ((*midProjMtx ).ldlt().solve((*DPWeightData).transpose() * (nu * (*DBWeight) + (mu / 2) * ((*DPWeightData) * (*SERVER_INIT_PWeight)))));
		}
	}
	else if (SwitchMethod.compare("AVG_SDH") == 0)                                  // AVG_SDH �p�����ǲ߼ҫ��ϥ�
	{
		*DPWeight            = *SERVER_INIT_PWeight;
	}
	else if (SwitchMethod.compare("PROX_FSDH") == 0)                                // PROX_FSDH �p�����ǲ߼ҫ��ϥ�
	{
		//ONE_HOT_ENCODING
		auto        uniqLabelsData    = uniqLabels_client(IBWeight);
		vector<int> uniqLabelList     = std::get<0>(uniqLabelsData);
		MatrixXi    IBWeightOneHot    = MatrixXi::Zero(IBWeight->rows(), LabelCount);
		// �ѩ� Matlab ���_�l�Ȥ��P�A�ҥH�ݦ���
		if (IBWeight->cols() == 1)
		{
			for (int i = 0; i < IBWeight->rows(); i++)
			{
				IBWeightOneHot(i, (*IBWeight)(i, 0) - 1) = 1;
				*DBWeight                                          = IBWeightOneHot.cast<double>(); // �N�쥻 int ���A B ������s�v�� �ഫ�� double ���A
			}
		}
		else
		{
			*DBWeight                                              = IBWeight->cast<double>();           // �N�쥻 int ���A B ������s�v�� �ഫ�� double ���A
		}


		//prjection matrix computing
		if (DPWeightData->rows() < DPWeightData->cols())
		{
			if (IBWeight->rows() < IBWeight->cols())
			{
				*midProjMtx  = ((*DPWeightData) * (*DPWeightData).transpose());
			}
			else
			{
				*midProjMtx  = ((*DPWeightData) * (*DPWeightData).transpose());
			}
			*finalProjMtx    = (*DPWeightData).transpose() * (*midProjMtx ).inverse();
			delete midProjMtx ;
			*DPWeight        = (2 / (2 * nu + mu)) * (((nu * (*DBWeight) + (mu / 2) * ((*DPWeightData) * (*SERVER_INIT_PWeight)))) * (*finalProjMtx ));
		}
		else
		{
			*midProjMtx = ((*DPWeightData).transpose() * (*DPWeightData));
			*DPWeight = (2 / (2 * nu + mu)) * ((*midProjMtx).ldlt().solve((*DPWeightData).transpose() * (nu * (*DBWeight) + (mu / 2) * ((*DPWeightData) * (*SERVER_INIT_PWeight)))));
		}
	}
	else if (SwitchMethod.compare("AVG_FSDH") == 0)                                // AVG_FSDH �p�����ǲ߼ҫ��ϥ�
	{
		*DPWeight           = *SERVER_INIT_PWeight;
	}
	// �����ʺA���t�O����
	delete SERVER_INIT_PWeight;
	delete midProjMtx ;
	delete finalProjMtx ;
	return std::make_tuple(DPWeight);
}

