/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
��s�C�ӽ����Ĥ@�^������s�v�� W �ϥ� WWeight_client.cpp ; �ĤG�^�H��ϥΦ���

�ܼƦW�� �}�Y�� D => DOUBLE ; I => INT ;
	Input Parameters
		----------  
		*DWWeightData              : array*  ; Ntran - by - C ; // �V�m��Ưx�}( B )                           
		*IWWeightOneHotLabels      : array*  ; C     - by - N ; // ���ո�Ưx�}( Y )  (�W���s�X)
		lambda                     : double  ;                  // SDH �����h�ưѼ�
		mu                         : double  ;                  // FedProx �����h�ưѼ�
		str_EXP                    : string  ;                  // ����X�^���㪺�ҫ���s
		str_ITR                    : string  ;                  // �̤j���A�ݻE�X����
		SwitchMethod               : string  ;                  // ��ܦb���Ӹ˸m�W�i���v����s(NANO , RASP) 
		weightPoolPath             : string  ;                  // ��m��s�v���θ�ƶ����@�ɸ�Ƨ�
	Output Parameters
		----------
		*DWWeight                  : array*  ; H     - by - L ; // ������s�v�� ( WWeight )
		start_WaitAggreWeight      : high_resolution_clock    ; // �������� Server �Ǧ^�E�X�v���}�l�ɶ��W
		end_WaitAggreWeight        : high_resolution_clock    ; // �������� Server �Ǧ^�E�X�v�������ɶ��W

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
#include "FileExists_client.h"
#include "uniqLabels_client.h"
#include "WWeightRRC_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


bool dirExists(const string& fileName)
{
	ifstream file(fileName);
	return file.good();
}

std::tuple<MatrixXd*, std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point> WWeightRRC_client(MatrixXd* DWWeightData, MatrixXi* IWWeightOneHotLabels, double lambda, double mu, string str_EXP, string str_ITR, string SwitchMethod, string weightPoolPath)
{
	Matrix<double, Dynamic, Dynamic>* DWWeight      = new Matrix<double, Dynamic, Dynamic>();                 // �ŧi W ������s�v��
	Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                 // �p���v�x�}�ɪ��L��x�} 1
	Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                 // �p���v�x�}�ɪ��L��x�} 2
	MatrixXd* DWWeightOneHotLabels                  = new MatrixXd(IWWeightOneHotLabels->cast<double>());      // �N�쥻 int ���A Label (�W���s�X)�ഫ�� double ���A

	// ���o Server �Ǧ^���E�X�x�}
	string SERVER_WWeightFile                       = weightPoolPath + str_EXP + "_" + str_ITR + INIT_CLIENT_WWeightMtxTxt;
	string SERVER_PWeightFile                       = weightPoolPath + str_EXP + "_" + str_ITR + INIT_CLIENT_PWeightMtxTxt;   
	auto start_WaitAggreWeight                      = std::chrono::high_resolution_clock::now();                              // �������� Server �Ǧ^�E�X�v���}�l�ɶ��W
	// �U���� while �j��O���� SERVER �ݶǰe������s�v���� NODE ��
	while ((get<0>(FileExists_client(SERVER_WWeightFile)) && (get<0>(FileExists_client(SERVER_PWeightFile)))) == false)
	{
		cout << "\t\t ��" + str_EXP + "_" + str_ITR + "����s\t";
		printf("Wait for the server sending WWeight...\n");
		// �C 1 �����@���ɮ׬O�_�s�b
		sleep_until(system_clock::now() + milliseconds(1000));
	}
	auto end_WaitAggreWeight = std::chrono::high_resolution_clock::now();                                                      // �������� Server �Ǧ^�E�X�v�������ɶ��W



	//Load SERVER AGGREGATED WWeight
	auto      Assign_WWeight      = LoadData_client(SERVER_WWeightFile);
	MatrixXd *SERVER_INIT_WWeight = new MatrixXd(std::get<0>(Assign_WWeight));

	// �̾ڿ���p�����ǲ߼ҫ������P ��ܤ��P����s��
	if (SwitchMethod.compare("PROX_SDH") == 0)                                  // PROX_SDH �p�����ǲ߼ҫ��ϥ�
	{
		//prjection matrix computing
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
	else if (SwitchMethod.compare("AVG_SDH") == 0)                                  // AVG_SDH �p�����ǲ߼ҫ��ϥ�
	{
		//*DWWeight            = *SERVER_INIT_WWeight;
		*DWWeight = (*SERVER_INIT_WWeight).transpose();
	}
	else if (SwitchMethod.compare("PROX_FSDH") == 0)                                // PROX_FSDH �p�����ǲ߼ҫ��ϥ�
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
			*midProjMtx      = ((*DWWeightOneHotLabels).transpose() * (*DWWeightOneHotLabels) + lambda * MatrixXd::Identity((*DWWeightOneHotLabels).cols(), (*DWWeightOneHotLabels).cols()) + MatrixXd::Zero((*DWWeightOneHotLabels).cols(), (*DWWeightOneHotLabels).cols()));
			*finalProjMtx    = midProjMtx ->ldlt().solve((*DWWeightOneHotLabels).transpose());
		}

		*DWWeight            = *finalProjMtx  * (*DWWeightData);
	}
	else if (SwitchMethod.compare("AVG_FSDH") == 0)                                // AVG_FSDH �p�����ǲ߼ҫ��ϥ�
	{
		*DWWeight            = *SERVER_INIT_WWeight;
	}
	// �����ʺA���t�O����
	delete SERVER_INIT_WWeight;
	delete midProjMtx ;
	delete finalProjMtx ;

	return std::make_tuple(DWWeight, start_WaitAggreWeight, end_WaitAggreWeight);
}


