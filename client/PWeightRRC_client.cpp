/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
更新每個輪次第一回局部更新權重 P 使用 PWeight_client.cpp ; 第二回以後使用此檔

變數名稱 開頭為 D => DOUBLE ; I => INT ;
	Input Parameters
		----------
		*DPWeightData     : array*  ; Ntran - by - L ; // 訓練資料矩陣( Phi(X) )
		*IBWeight         : array*  ; Ntran - by - C ; // 測試資料矩陣( B )
		lambda            : double  ;                  // SDH 的正則化參數
		nu                : double  ;                  // 懲罰參數
		mu                : double  ;                  // FedProx 的正則化參數

	Output Parameters
		----------
		*DPWeight         : array*  ; H     - by - L ; // 局部權重 ( PWeight )


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

	Matrix<double, Dynamic, Dynamic>* DPWeight      = new Matrix<double, Dynamic, Dynamic>();                 // 宣告 P 局部更新權重         變數名稱 開頭為 D => DOUBLE ; I => INT ;
	Matrix<double, Dynamic, Dynamic>* DBWeight      = new Matrix<double, Dynamic, Dynamic>();                 // 宣告 B 局部更新權重
	Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                 // 計算投影矩陣時的過渡矩陣 1
	Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                 // 計算投影矩陣時的過渡矩陣 2



	//Load SERVER AGGREGATED PWeight
    auto     Assign_PWeight                         = LoadData_client(LoadDataPath + str_EXP + "_" + str_ITR + INIT_CLIENT_PWeightMtxTxt);
	MatrixXd *SERVER_INIT_PWeight                   = new MatrixXd(std::get<0>(Assign_PWeight));

	//選擇使用何種模型
	if (SwitchMethod.compare("PROX_SDH") == 0)                                                                // PROX_SDH 聯邦式學習模型使用
	{
		//ONE_HOT_ENCODING
		auto        uniqLabelsData                  = uniqLabels_client(IBWeight);
		vector<int> uniqLabelList                   = std::get<0>(uniqLabelsData);
		MatrixXi    IBWeightOneHot                  = MatrixXi::Zero(IBWeight->rows(), LabelCount);
		// 由於 Matlab 的起始值不同，所以需扣掉 1 
		if (IBWeight->cols() == 1)
		{
			for (int i = 0; i < IBWeight->rows(); i++)
			{
				IBWeightOneHot(i, (*IBWeight)(i, 0) - 1) = 1;
				*DBWeight                                          = IBWeightOneHot.cast<double>();           // 將原本 int 型態 B 局部更新權重 轉換成 double 型態
			}
		}
		else
		{
			*DBWeight                                              = IBWeight->cast<double>();                // 將原本 int 型態 B 局部更新權重 轉換成 double 型態
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
	else if (SwitchMethod.compare("AVG_SDH") == 0)                                  // AVG_SDH 聯邦式學習模型使用
	{
		*DPWeight            = *SERVER_INIT_PWeight;
	}
	else if (SwitchMethod.compare("PROX_FSDH") == 0)                                // PROX_FSDH 聯邦式學習模型使用
	{
		//ONE_HOT_ENCODING
		auto        uniqLabelsData    = uniqLabels_client(IBWeight);
		vector<int> uniqLabelList     = std::get<0>(uniqLabelsData);
		MatrixXi    IBWeightOneHot    = MatrixXi::Zero(IBWeight->rows(), LabelCount);
		// 由於 Matlab 的起始值不同，所以需扣掉
		if (IBWeight->cols() == 1)
		{
			for (int i = 0; i < IBWeight->rows(); i++)
			{
				IBWeightOneHot(i, (*IBWeight)(i, 0) - 1) = 1;
				*DBWeight                                          = IBWeightOneHot.cast<double>(); // 將原本 int 型態 B 局部更新權重 轉換成 double 型態
			}
		}
		else
		{
			*DBWeight                                              = IBWeight->cast<double>();           // 將原本 int 型態 B 局部更新權重 轉換成 double 型態
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
	else if (SwitchMethod.compare("AVG_FSDH") == 0)                                // AVG_FSDH 聯邦式學習模型使用
	{
		*DPWeight           = *SERVER_INIT_PWeight;
	}
	// 消除動態分配記憶體
	delete SERVER_INIT_PWeight;
	delete midProjMtx ;
	delete finalProjMtx ;
	return std::make_tuple(DPWeight);
}

