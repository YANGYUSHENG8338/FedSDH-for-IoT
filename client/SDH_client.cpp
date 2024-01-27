/*
@author:     Yu-Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo-Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    
Log: 0.0.1 Initial
     0.0.2 改成指標形式
     0.0.3 FedProxSDH的Q移除Bpre
	       FedProxFSDH的UpdateIBWeight移除Bpre
	 0.0.4 sigma和Gmap_Lambda改double [BWC]
	 
	Input Parameters
		----------
		*X               : array*  ; Ntran - by - Ntran ; 訓練 Phi 資料
		*y               : array*  ; Ntran - by - 1     ; 訓練資料標籤
		*B               : array*  ; Ntran - by - L     ; 初始 B 資料
		Prox_Mu          : double  ; FedProx 的正則化參數
		Gmap_Lambda      : double  ; G-STEP-lambda      求WWeight的Ridge Param
		Gmap_loss        : str     ; L2
		Fmap_type        : str     ; SDH_Function
		Fmap_Nu          : double  ; SDH 的正則化參數
		Fmap_Lambda      : double  ; F-STEP-lambda      求PWeight的Ridge Param
		LocalIteration   : double  ; 局部權重更新次數
		tol              : double  ; bias 參數
		exp              : int     ; 執行幾回完整的模型更新
		itr              : int     ; 最大伺服端聚合輪次
		*Bpre            : array*  ; Ntran - by - L     ; 前一聚合輪次的 B 矩陣
		SwitchMethod     : string  ; 選擇在哪個裝置上進行權重更新(NANO , RASP)
		weightPoolPath   : string  ; 放置更新權重及資料集的共享資料夾
	Output Parameters
		----------
		*WWeight                      : array*                ; L     - by - C ; G-STEP-權重
		*PWeight                      : array*                ; Ntran - by - L ; F-STEP-權重
		*B                            : array                 ; Ntran - by - L ; 更新B資料
		start_WaitAggreWeight         : high_resolution_clock ; 紀錄等待 Server 傳回聚合權重開始時間戳
		end_WaitAggreWeight           : high_resolution_clock ; 紀錄等待 Server 傳回聚合權重結束時間戳
*/
#pragma warning( disable : 4996 ) 
#include <iostream>
#include <stdio.h>
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
#include "LoadData_client.h"
#include "WWeight_client.h"
#include "PWeight_client.h"
#include "WWeightRRC_client.h"
#include "PWeightRRC_client.h"
#include "double2txt_client.h"
#include "uniqLabels_client.h"
#include "GetCurrentTimeStamp_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


typedef struct args_struct {
	int LinFormWght;
	int QuadFormWght;
}argu;

void RemoveRow(MatrixXd* matrix, unsigned int rowToRemove) {
	unsigned int numRows = matrix->rows() - 1;
	unsigned int numCols = matrix->cols();

	if (rowToRemove < numRows) {
		matrix->block(rowToRemove, 0, numRows - rowToRemove, numCols) = matrix->block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);
	}

	matrix->conservativeResize(numRows, numCols);
}
void RemoveColumn(MatrixXd* matrix, unsigned int colToRemove) {
	unsigned int numRows = matrix->rows();
	unsigned int numCols = matrix->cols() - 1;

	if (colToRemove < numCols) {
		matrix->block(0, colToRemove, numRows, numCols - colToRemove) = matrix->block(0, colToRemove + 1, numRows, numCols - colToRemove);
	}

	matrix->conservativeResize(numRows, numCols);
}




std::tuple<
           MatrixXd*, MatrixXd*, MatrixXi*, 
		   std::chrono::high_resolution_clock::time_point, 
		   std::chrono::high_resolution_clock::time_point
		  > 
SDH_client
          (
           MatrixXd* X   , MatrixXi* y        , MatrixXi* B        , 
		   double Prox_Mu, double Gmap_Lambda , string Gmap_loss   , string Fmap_type, double Fmap_Nu, double Fmap_Lambda, 
		   double tol    , int LocalIteration , int debug          , int exp         , int itr       , 
		   MatrixXd* Bpre, string SwitchMethod, string LoadDataPath, int LabelCount
		  )
{
	// 參數設定
	int         N              = B->rows();
	int         L              = B->cols();
	int         localitr       = 0;                               // 初始化局部權重更新次數
	//Proximal Term懲罰參數
	double      mu             = Prox_Mu;                         // FedProx 正則化參數
	double      nu             = Fmap_Nu;                         // 懲罰參數
	double      detla          = 1 / nu;
	tol                        = 1e-7;                            // bias 參數
	string      str_EXP        = to_string(exp);                  // 執行幾回完整的模型更新
	string      str_ITR        = to_string(itr);                  // 最大伺服端聚合輪次

	//argument
	argu        args_value     = { 1, 0};
	double      linFormWght    = args_value.LinFormWght;
	int         quadFormWght   = args_value.QuadFormWght;
	MatrixXi    LinForm        = MatrixXi::Zero(N, 1);

	//ONE_HOT_ENCODING
	//Label matrix N x c
	auto        uniqLabelsData = uniqLabels_client(y);
	vector<int> uniqLabelList  = std::get<0>(uniqLabelsData);
	MatrixXi*   IyOneHotLabel  = new MatrixXi(MatrixXi::Zero(y->rows(), LabelCount));
	// 由於 Matlab 的起始值不同，所以需扣掉 1
	if (y->cols() == 1)
	{
		for (int i = 0; i < y->rows(); i++)
		{
			(*IyOneHotLabel)(i, (*y)(i, 0) - 1) = 1;
		}
	}
	else
	{
		*IyOneHotLabel = *y;
	}

	MatrixXd RenewLabelList = MatrixXd::Zero(uniqLabelList.size(), 1); // 創建置放標籤有哪幾類的矩陣
	// 取得該裝置得到標籤有哪幾類，並將其輸入矩陣中 
	for (int RenewLabelListIdx = 0; RenewLabelListIdx < uniqLabelList.size(); RenewLabelListIdx++)
	{
		RenewLabelList(RenewLabelListIdx, 0) = uniqLabelList[RenewLabelListIdx];
	}



	std::chrono::high_resolution_clock::time_point start_WaitAggreWeight, end_WaitAggreWeight;     // 宣告紀錄等待 Server 傳回聚合權重的時間戳
	Matrix<double, Dynamic, Dynamic>* WWeight    = new Matrix<double, Dynamic, Dynamic>();         // 局部更新權重 ( WWeight )
	Matrix<double, Dynamic, Dynamic>* PWeight    = new Matrix<double, Dynamic, Dynamic>();         // 局部更新權重 ( PWeight )
	Matrix<double, Dynamic, Dynamic>* WF0        = new Matrix<double, Dynamic, Dynamic>();         // 更新局部權重時門檻值用(初始 PWeight)
	MatrixXd* DBWeight                           = new MatrixXd(B->cast<double>());                // 局部更新後的 B 權重(int    型態)
	MatrixXi* UpdateIBWeight                     = nullptr;                                        // 局部更新後的 B 權重(double 型態)
	MatrixXd* UpdateDBWeight                     = nullptr;                // 將原本 int 型態 訓練資料標籤(ttran) 局部更新權重 轉換成 double 型態
	MatrixXd* DyOneHotLabel                      = new MatrixXd(IyOneHotLabel->cast<double>());
	
	//G-STEP
	if (Gmap_loss == "L2")
	{
		if (itr == 0)                             // 產生初始 W 局部更新權重 
		{
			auto Wg                = WWeight_client(DBWeight, IyOneHotLabel, Gmap_Lambda, SwitchMethod);
			WWeight                = get<0>(Wg);
		}
		else                                      // 產生第二輪以後 W 的局部更新權重 
		{
			auto Wg                = WWeightRRC_client(DBWeight, IyOneHotLabel, Gmap_Lambda, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath);
			WWeight                = get<0>(Wg);
			start_WaitAggreWeight  = get<1>(Wg);  // 紀錄等待 Server 傳回聚合權重開始時間戳
			end_WaitAggreWeight    = get<2>(Wg);  // 紀錄等待 Server 傳回聚合權重結束時間戳
		}
	}
	//F-STEP
	if (itr == 0)                                 // 產生初始 P 局部更新局部權重 
	{
		auto WF                    = PWeight_client(X, B, Fmap_Lambda, LabelCount);
		PWeight                    = get<0>(WF);
		WF0                        = get<0>(WF);
	}
	else                                          // 產生第二輪以後 W 的局部更新權重
	{
		auto WF                    = PWeightRRC_client(X, B, nu, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath, LabelCount);
		PWeight                    = get<0>(WF);
		WF0                        = get<0>(WF);
	}

	while (localitr < LocalIteration)
	{
		localitr++;
		if ((SwitchMethod == "PROX_SDH") || (SwitchMethod == "AVG_SDH"))
		{
			//B-STEP
			MatrixXd* XF   = new MatrixXd((*X) * (*PWeight));
			MatrixXd* Q    = new MatrixXd(MatrixXd::Zero(B->rows(), B->cols()));
			//Load SERVER INIT B
			if (itr == 0)  // 產生第一輪 Q 局部更新局部權重
			{
				*Q   = nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
			}
			else           // 產生第二輪以後 Q 局部更新局部權重
			{
                if (SwitchMethod == "AVG_SDH")
                {
                    *Q   = nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
                }
                else if(SwitchMethod == "PROX_SDH")
                {
					//育勝版 用Bpre應有誤
                    //*Q   = (mu / 2) * (*Bpre) + nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
				    //修正版 20230726
				    *Q   =                      nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
                }
            }
			for (int time = 0; time < 10; time++)
			{
				Bpre = new MatrixXd(B->cast<double>());
				for (int k = 0; k < B->cols(); k++)
				{
					MatrixXd* Zk  = new MatrixXd(B->cast<double>());
					RemoveColumn(Zk, k);
					MatrixXd* Wkk = new MatrixXd((*WWeight).row(k));
					MatrixXd* Wk  = new MatrixXd(*WWeight);;
					RemoveRow(Wk, k);
					//LinForm N-by-1
					MatrixXd* LinForm = new MatrixXd((*Q).col(k) - (*Zk) * (*Wk) * (*Wkk).transpose());
					if (quadFormWght > 0)
					{
						quadFormWght++;
					}
					else
					{
						B->col(k) = (linFormWght * (*LinForm)).array().sign().matrix().cast<int>();  // 產生局部更新後的 B
					}
					if (quadFormWght > 0)
					{
						//cout << 'b' << endl;
					}
					// 消除動態分配記憶體
					delete Zk;
					delete Wkk;
					delete Wk;
					delete LinForm;
				}
				UpdateDBWeight = new MatrixXd(B->cast<double>());
				UpdateIBWeight = B;
				if (((*UpdateDBWeight) - (*Bpre)).squaredNorm() < 1e-6 * (*Bpre).squaredNorm())
				{
					break;
				}
				// 消除動態分配記憶體
				delete Bpre;
			}
			// 消除動態分配記憶體
			delete XF;
			delete Q;
		}
		else if ((SwitchMethod == "PROX_FSDH") || (SwitchMethod == "AVG_FSDH"))
		{
			//B-STEP
			MatrixXd* FSDH_XF = new MatrixXd((*X) * (*PWeight));
			if (itr == 0)
			{
				UpdateIBWeight = new MatrixXi(((*DyOneHotLabel) * (*WWeight) + nu * (*FSDH_XF)).array().sign().matrix().cast<int>());  // 產生局部更新後的 B
			}
			else
			{
                if (SwitchMethod == "AVG_FSDH")
                {
                    UpdateIBWeight = new MatrixXi(((*DyOneHotLabel) * (*WWeight) + nu * (*FSDH_XF)).array().sign().matrix().cast<int>());  // 產生局部更新後的 B
                }
                else if(SwitchMethod == "PROX_FSDH")
                {
					//育勝版 用Bpre應有誤
				    //UpdateIBWeight = new MatrixXi(((mu / 2) * (*Bpre) + nu * (*FSDH_XF) + (*DyOneHotLabel) * (*WWeight)).array().sign().matrix().cast<int>());  // 產生局部更新後的 B
				    //修正版 20230726
				    UpdateIBWeight = new MatrixXi((                     nu * (*FSDH_XF) + (*DyOneHotLabel) * (*WWeight)).array().sign().matrix().cast<int>());  // 產生局部更新後的 B
                }
			}
			UpdateDBWeight = new MatrixXd(UpdateIBWeight->cast<double>());
		}
		//G-STEP
		if (Gmap_loss == "L2")
		{
			if (itr == 0)
			{
				auto Wg = WWeight_client(UpdateDBWeight, IyOneHotLabel, Gmap_Lambda, SwitchMethod);
				WWeight = get<0>(Wg);
			}
			else
			{
				auto Wg = WWeightRRC_client(UpdateDBWeight, IyOneHotLabel, Gmap_Lambda, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath);
				WWeight = get<0>(Wg);
			}

		}
		//F-STEP
		if (itr == 0)
		{
			auto WF = PWeight_client(X, UpdateIBWeight, Fmap_Lambda, LabelCount);
			PWeight = get<0>(WF);
		}
		else
		{
			auto WF = PWeightRRC_client(X, UpdateIBWeight, nu, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath, LabelCount);
			PWeight = get<0>(WF);
		}
		//更新後的B再賦值給UpdateDBWeight
		double bias = ((*UpdateDBWeight) - (*X) * (*PWeight)).squaredNorm();
		if (debug)
		{
			//cout << "bias = " << bias << endl;
		}

		if (bias < tol * (*UpdateDBWeight).squaredNorm())
		{
			break;
		}

		if ((*PWeight - *WF0).squaredNorm() < tol * (*WF0).squaredNorm())
		{
			break;
		}

	}
	// LoadDataPath = "..\\weightPoolPath\\SERVER2NANO_"
	string cutDataPath_1 = LoadDataPath.substr(0, LoadDataPath.length() - 12);
	string cutDataPath_2 = LoadDataPath.substr(LoadDataPath.length() - 5 , 4);
	string renewDataPath = cutDataPath_1 + cutDataPath_2 + "2SERVER_";
	//儲存更新之權重
	string CLIENT_to_SEVER_WWeight   = renewDataPath + str_EXP + "_" + str_ITR + INIT_CLIENT_WWeightMtxTxt;
	string CLIENT_to_SEVER_PWeight   = renewDataPath + str_EXP + "_" + str_ITR + INIT_CLIENT_PWeightMtxTxt;
    string CLIENT_to_SEVER_LabelList = renewDataPath + str_EXP + INIT_CLIENT_LabelListMtxTxt;
	// 傳回 Server 端
	if ((SwitchMethod == "PROX_SDH") || (SwitchMethod == "AVG_SDH"))
	{
		//MatrixXd* SDH_WWeight = new MatrixXd(std::get<0>(WWeight));
		MatrixXd* SDH_WWeight = new MatrixXd((*WWeight).transpose());
		double2txt_client(SDH_WWeight, CLIENT_to_SEVER_WWeight);
	}
	else if ((SwitchMethod == "PROX_FSDH") || (SwitchMethod == "AVG_FSDH"))
	{
		double2txt_client(WWeight, CLIENT_to_SEVER_WWeight);

	}

	//double2txt_client(WWeight        , CLIENT_to_SEVER_WWeight);
	double2txt_client(PWeight        , CLIENT_to_SEVER_PWeight);
    if (itr == 0)
    {
        double2txt_client(&RenewLabelList, CLIENT_to_SEVER_LabelList);
    }

	


	return std::make_tuple(WWeight, PWeight, B, start_WaitAggreWeight, end_WaitAggreWeight);

}
