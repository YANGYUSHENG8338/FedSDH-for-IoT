/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
更新每個輪次第一回局部更新權重 W 使用 WWeight_client.cpp ; 第二回以後使用此檔

變數名稱 開頭為 D => DOUBLE ; I => INT ;
	Input Parameters
		----------  
		*DWWeightData              : array*  ; Ntran - by - C ; // 訓練資料矩陣( B )                           
		*IWWeightOneHotLabels      : array*  ; C     - by - N ; // 測試資料矩陣( Y )  (獨熱編碼)
		lambda                     : double  ;                  // SDH 的正則化參數
		mu                         : double  ;                  // FedProx 的正則化參數
		str_EXP                    : string  ;                  // 執行幾回完整的模型更新
		str_ITR                    : string  ;                  // 最大伺服端聚合輪次
		SwitchMethod               : string  ;                  // 選擇在哪個裝置上進行權重更新(NANO , RASP) 
		weightPoolPath             : string  ;                  // 放置更新權重及資料集的共享資料夾
	Output Parameters
		----------
		*DWWeight                  : array*  ; H     - by - L ; // 局部更新權重 ( WWeight )
		start_WaitAggreWeight      : high_resolution_clock    ; // 紀錄等待 Server 傳回聚合權重開始時間戳
		end_WaitAggreWeight        : high_resolution_clock    ; // 紀錄等待 Server 傳回聚合權重結束時間戳

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
	Matrix<double, Dynamic, Dynamic>* DWWeight      = new Matrix<double, Dynamic, Dynamic>();                 // 宣告 W 局部更新權重
	Matrix<double, Dynamic, Dynamic>* midProjMtx    = new Matrix<double, Dynamic, Dynamic>();                 // 計算投影矩陣時的過渡矩陣 1
	Matrix<double, Dynamic, Dynamic>* finalProjMtx  = new Matrix<double, Dynamic, Dynamic>();                 // 計算投影矩陣時的過渡矩陣 2
	MatrixXd* DWWeightOneHotLabels                  = new MatrixXd(IWWeightOneHotLabels->cast<double>());      // 將原本 int 型態 Label (獨熱編碼)轉換成 double 型態

	// 取得 Server 傳回的聚合矩陣
	string SERVER_WWeightFile                       = weightPoolPath + str_EXP + "_" + str_ITR + INIT_CLIENT_WWeightMtxTxt;
	string SERVER_PWeightFile                       = weightPoolPath + str_EXP + "_" + str_ITR + INIT_CLIENT_PWeightMtxTxt;   
	auto start_WaitAggreWeight                      = std::chrono::high_resolution_clock::now();                              // 紀錄等待 Server 傳回聚合權重開始時間戳
	// 下面的 while 迴圈是等待 SERVER 端傳送全局更新權重到 NODE 端
	while ((get<0>(FileExists_client(SERVER_WWeightFile)) && (get<0>(FileExists_client(SERVER_PWeightFile)))) == false)
	{
		cout << "\t\t 第" + str_EXP + "_" + str_ITR + "次更新\t";
		printf("Wait for the server sending WWeight...\n");
		// 每 1 秒偵測一次檔案是否存在
		sleep_until(system_clock::now() + milliseconds(1000));
	}
	auto end_WaitAggreWeight = std::chrono::high_resolution_clock::now();                                                      // 紀錄等待 Server 傳回聚合權重結束時間戳



	//Load SERVER AGGREGATED WWeight
	auto      Assign_WWeight      = LoadData_client(SERVER_WWeightFile);
	MatrixXd *SERVER_INIT_WWeight = new MatrixXd(std::get<0>(Assign_WWeight));

	// 依據選擇聯邦式學習模型的不同 選擇不同的更新式
	if (SwitchMethod.compare("PROX_SDH") == 0)                                  // PROX_SDH 聯邦式學習模型使用
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
	else if (SwitchMethod.compare("AVG_SDH") == 0)                                  // AVG_SDH 聯邦式學習模型使用
	{
		//*DWWeight            = *SERVER_INIT_WWeight;
		*DWWeight = (*SERVER_INIT_WWeight).transpose();
	}
	else if (SwitchMethod.compare("PROX_FSDH") == 0)                                // PROX_FSDH 聯邦式學習模型使用
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
	else if (SwitchMethod.compare("AVG_FSDH") == 0)                                // AVG_FSDH 聯邦式學習模型使用
	{
		*DWWeight            = *SERVER_INIT_WWeight;
	}
	// 消除動態分配記憶體
	delete SERVER_INIT_WWeight;
	delete midProjMtx ;
	delete finalProjMtx ;

	return std::make_tuple(DWWeight, start_WaitAggreWeight, end_WaitAggreWeight);
}


