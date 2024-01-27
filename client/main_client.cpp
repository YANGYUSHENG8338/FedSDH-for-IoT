/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    
Log: 0.0.1 Initial
     0.0.2 改成指標形式
	 0.0.3 sigma和Gmap_Lambda改double [BWC]
	       增加引數 SDHWrapper_client.cpp double SDH_FmapNu  ,   double  SDH_GmapLambda,    double  SDH_FmapLambda 
	 
指定路徑
	SERVER
	..\\weightPoolPath\\*******.txt                            // 資料集、標籤集和權重存放位置
	RASP
	/home/raspdslab/weightPoolPath/*******.txt                 // 資料集、標籤集和權重存放位置
	NANO
	/home/bn10513137/weightPoolPath/*******.txt                // 資料集、標籤集和權重存放位置


OneAggre - Segment

	RASP - Power
	/home/raspdslab/evaPoolPath/INA219.py                     // 主程式 - 取用 Power 數據
	/home/raspdslab/evaPoolPath/logOneAggrePower.txt          // 印出掃描 Power 數據 txt 檔
	/home/raspdslab/evaPoolPath/calcOneAggrePower.py          // 計算 Power 數據 程式
	/home/raspdslab/evaPoolPath/calcOneAggrePower.txt         // 印出總和 Power 相關數據(平均功耗) txt 檔

	NANO - Power
	/home/bn10513137/evaPoolPath/logOneAggrePower.py          // 主程式 - 取用 Power 數據
	/home/bn10513137/evaPoolPath/logOneAggrePower.txt         // 印出掃描 Power 數據 txt 檔
	/home/bn10513137/evaPoolPath/calcOneAggrePower.py         // 計算 Power 數據 程式
	/home/bn10513137/evaPoolPath/calcOneAggrePower.txt        // 印出總和 Power 相關數據(平均功耗) txt 檔

	RASP - Memory
	/home/raspdslab/evaPoolPath/logOneAggreMemory.py          // 主程式 - 取用 Memory 數據
	/home/raspdslab/evaPoolPath/logOneAggreMemory.txt         // 印出掃描 Memory 數據 txt 檔
	/home/raspdslab/evaPoolPath/calcOneAggreMemory.py         // 計算 Memory 數據 程式
	/home/raspdslab/evaPoolPath/calcOneAggreMemory.txt        // 印出總和 Memory 相關數據(平均功耗) txt 檔

	NANO - Memory
	/home/bn10513137/evaPoolPath/logOneAggreMemory.py         // 主程式 - 取用 Memory 數據
	/home/bn10513137/evaPoolPath/logOneAggreMemory.txt        // 印出掃描 Memory 數據 txt 檔
	/home/bn10513137/evaPoolPath/calcOneAggreMemory.py        // 計算 Memory 數據 程式
	/home/bn10513137/evaPoolPath/calcOneAggreMemory.txt       // 印出總和 Memory 相關數據(平均功耗) txt 檔
*/ 

#pragma warning( disable : 4996 )
#ifdef _WIN64 // Windows x64 環境下使用
#include <windows.h>
#include <filesystem>
#else// Linux 環境下使用
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

#endif

#include <stdio.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <ctime>
#include <random>
#include <numeric>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "main_client.h"
#include "meanstd_client.h"
#include "meanstdPerAggreIter_client.h"
#include "SDHWrapper_client.h"
#include "getLabel_client.h"
#include "evaluate_client.h"
#include "multilabelmaPrecision_client.h"
#include "getAnchor_client.h"
#include "LoadData_client.h"
#include "FileExists_client.h"
#include "uniqLabels_client.h"
#include "dispMsg_client.h"
#include "double2txt_client.h"
#include "EmptyWeightPool_client.h"
#include "RemoveEvaLogFile_client.h"
#include "GetCurrentTimeStamp_client.h"
#include "pythonRunner_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds




struct param_struct {
	int    nExps;                                    // 執行幾回完整的模型更新
	int    J;                                        // 錨點
	int    L;                                        // 編碼長度
	int    AggreIter;                                // 最大伺服端聚合輪次
	int    LocalIteration;                           // 最大伺服端聚合輪次
}param;

typedef struct SDH_Kernel_struct {
	String   Function;                               // 使用何者 Kernel (RBF Kernel)
	double   RBFsigma;                               // 計算 Phi 值所需的 sigma
	int      HammingDist;                            // 漢明距離
	double   ProxMu;                                 // FedProx 正則化參數
    double   FmapNu;                                 // 懲罰參數 Fmap_Nu * ||B - P'Phi||_2	        
    double   GmapLambda;                             // G-STEP Lambda   求WWeight的Ridge Param  (Originally in SDHWrapper_client.cpp
    double   FmapLambda;                             // F-STEP Lambda   求PWeight的Ridge Param  (Originally in SDHWrapper_client.cpp
}SDH_Kernel;







extern int main(int argc, char** argv)
{
	// 參數設定 NANO_train&test_SPEED_FedProxFSDH_JAFFE_EXP10_ITR5_16B_SI1_MU10_5_1
    string datasetnam         = "ORL";                              // 測試資料集("YALE", "JAFFE", "ORL", "SMALL_ARFACE")
	string SwitchDevice       = "RASP";                             // 選擇在哪個裝置上進行權重更新(NANO , RASP)  
    string SwitchMethod       = "AVG_SDH";                          // 選擇何種聯邦學習式模型(AVG_SDH, AVG_FSDH, PROX_SDH 和 PROX_FSDH)
    string SwitchSegment      = "ALL";                              // 選擇量測完整一回模型更新區段或是在模型訓練或模型測試區段進行(ALL 和 TRAINTEST)
    string SwitchEvaluation   = "MEMORY";                           // 選擇量測何種評估指標  (SPEED, POWER 和 MEMORY)
	string switchEval_METHOD  = "micro";                            // Accuracy, Precision, Recall 和 F1 使用 micro 平均; macro
	vector<int> bitlen        = { 16, 32 , 64, 96, 128};            // (只允許一次一種碼長 因有bug)程式中須測試的碼長，主要碼長設定在param.L;   16, 32 , 64, 96, 128
	int nChannels             = 1;                                  // if RGB, nChannels = 3
    param.nExps               = 1;                                  // 執行幾回完整的模型更新
	param.J                   = 0;                                  // 錨點; 0 means all training data
	param.L                   = 32;                                 // (無效 因會被bitlen設定) 編碼長度(先宣告，32為預設值) 
	param.AggreIter           = 5;                                  // 最大伺服端聚合輪次
	param.LocalIteration      = 10;                                 // (無效 因會被LocalIterationSDH, LocalIterationFSDH設定) 局部權重更新次數
	int LocalIterationSDH     = 20;                                 // SDH 專用局部權重更新次數   (會設定param.LocalIteration)
	int LocalIterationFSDH    = 3;                                  // FSDH 專用局部權重更新次數  (會設定param.LocalIteration)
	int timeout_wait4WP_limit = 168;                                // 當掃描此資料集是否有指定權重的上限次數
	SDH_Kernel SDH_K          = {"RBF", 1, 2, 1e-7 ,1e-5 ,1 ,1e-2}; // 請參照結構變數 SDH_Kernel_struct
	MatrixXd* Bpre            = nullptr;                            // 前一聚合輪次的 B 矩陣
	stringstream ProxMu;                                            // 宣告變數 ProxMu 為字串流形式
	ProxMu << scientific << setprecision(1) << SDH_K.ProxMu;        // 將浮點數 assign 給字串流(將浮點數轉換為科學記號的字串形式)
	// 變數宣告
	string weightPoolPath, evaPoolPath;                             // 查詢此路徑下資料夾的聚合權重是否清空 ; 放置 POWER 或 MEMORY 相關資料
	string XtranPath, XtestPath, LtranPath, LtestPath, LabelCountPath; // 存取 SERVER 端分發的資料集，LabelCountPath (傳來 Label 類別總共有有多少個)
	vector<double> Eva_acc, Eva_pre, Eva_rec, Eva_f1, Eva_mAP, Eva_TranTime, Eva_TestTime, Eva_OneAggreTime; //儲存每輪局部更新完獲得的各項評估指標，Eva_OneAggreTime(包含 train 和 test)
	string calcPython, calcSavePath, calcPythonCommand;             // 計算 POWER 或 MEMORY 的 python 檔 的指令 ; 儲存 POWER 或 MEMORY 計算後相關數據的檔案路徑 ; 代入參數計算指令(calcPython + 參數)

	// 選擇何種裝置路徑，還有查詢此路徑下資料夾的聚合權重是否清空
	#ifdef _WIN64 // Windows x64 環境下使用
	{
		weightPoolPath = INIT_x64CLIENT_weightPoolPath; // ### 都用 / 當路徑
	}
	#else // Linux 環境下使用
	{
		if (SwitchDevice.compare("NANO") == 0)
		{
            weightPoolPath = INIT_NANOCLIENT_weightPoolPath; // bn10513137 為NANO OS 使用者名稱
			evaPoolPath    = INIT_NANOCLIENT_evaPoolPath;    // bn10513137 為NANO OS 使用者名稱  
		}
		else if (SwitchDevice.compare("RASP") == 0)
		{
			weightPoolPath = INIT_RASPCLIENT_weightPoolPath; // raspdslab 為RASP OS使用者名稱
			evaPoolPath    = INIT_RASPCLIENT_evaPoolPath;    // raspdslab 為RASP OS使用者名稱
		}
	}
	#endif



	string LoadDataPath = weightPoolPath + INIT_CLIENT_SERVER2 + SwitchDevice + "_";      //      LoadDataPath = "../weightPoolPath/SERVER2NANO_"
    cout << LoadDataPath << endl;

    //TreadName2LogMemory.py
    string ThreadName = evaPoolPath + INIT_CLIENT_threadNamePath;
    std::ofstream Startfile1(ThreadName);  // 調解SERVER 和 CLIENT 清空資料夾的時機
    if (Startfile1.is_open()) {  // 確保文件成功打開
        Startfile1 << INIT_CLIENT_threadName << std::endl;
        Startfile1.close();  // 關閉文件
        cout << "字串已成功寫入 ControlSERVERNANOFile。" << std::endl;
    }
    else {
        cout << "無法打開 ControlSERVERNANOFile。" << std::endl;
    }

	for (int testBitIdx = 0; testBitIdx < bitlen.size(); testBitIdx++)
	{
		if (testBitIdx < 1)
		{
            EmptyWeightPool_client(weightPoolPath);                               // 查詢此資料夾是否清空 (EmptyWeightPool_client 只能在 linux 環境下跑)
			RemoveEvaLogFile_client(evaPoolPath, SwitchSegment, SwitchEvaluation);// 刪除前一個測試碼長留下的 Power 和 Memory 的 紀錄檔案 (logOneAggrePower.txt , logTrainPower.txt, logTestPower.txt, logOneAggreMemory.txt , logTrainMemory.txt, logTestMemory.txt)
		}

		param.L = bitlen[testBitIdx];                          // 編碼長度
		// SPEED_NANO_JAFFE_TRAINTEST_FedProx_FSDH_EXP10_ITR5_16B_SI1_MU10e-5.txt
		string disp2filePath = SwitchEvaluation + "_" + SwitchDevice + "_" + datasetnam + "_" + SwitchSegment + INIT_CLIENT_Fed + SwitchMethod + INIT_CLIENT_EXP + to_string(param.nExps) + INIT_CLIENT_ITR + to_string(param.AggreIter) + "_" + to_string(bitlen[testBitIdx]) + INIT_CLIENT_B_SI + to_string(SDH_K.RBFsigma) + INIT_CLIENT_MU + ProxMu.str() + INIT_CLIENT_txt;

		//將終端機所有輸出寫入一個文檔中
		auto recordOutputData  = dispMsg_client(disp2filePath);  
		FILE* f                = std::get<0>(recordOutputData); // 將所有 fprintf 的輸出寫入文檔中
		//streambuf* coutbuf     = std::get<1>(recordOutputData); // 將所有 cout    的輸出寫入文檔中


		if      ((SwitchMethod.compare("PROX_SDH" ) == 0) || (SwitchMethod.compare("AVG_SDH" ) == 0))
		{
			param.LocalIteration = LocalIterationSDH;
		}
		else if ((SwitchMethod.compare("PROX_FSDH") == 0) || (SwitchMethod.compare("AVG_FSDH") == 0))
		{
			param.LocalIteration = LocalIterationFSDH;
		}




		// 將輸出寫入文檔SwitchMethod
		fprintf(f, "DataName        :\t\t % s\n" , datasetnam.c_str()   );
		fprintf(f, "SwitchMethod    :\t\t % s\n" , SwitchMethod.c_str() );
		fprintf(f, "nExps           :\t\t % 3d\n", param.nExps          );
		fprintf(f, "L               :\t\t % 3d\n", param.L              );
		fprintf(f, "AggreIter       :\t\t % 3d\n", param.AggreIter      );
		fprintf(f, "LocalIteration  :\t\t % 3d\n", param.LocalIteration );
		fprintf(f, "RBFSIGMA        :\t\t % lf\n", SDH_K.RBFsigma       );
		fprintf(f, "HAMMRADIUS      :\t\t % 3d\n", SDH_K.HammingDist    );
		fprintf(f, "ProxMu          :\t\t % s\n" , ProxMu.str().c_str() );
		





		// 記錄整個程式開始時間
		auto startElapsedTime = high_resolution_clock::now();

		//int iteration = 5;
		for (int expid = 0; expid < param.nExps; expid++)
		{

			for (int aggreItr = 0; aggreItr < param.AggreIter; aggreItr++)
			{
				// 記錄模型一回聚合回合開始時間 (print 使用)
				auto startAggreItr = system_clock::now();
				auto legacyStart   = system_clock::to_time_t(startAggreItr);

				printf  (   "Experiment:Iter % .3d / % .3d : % .3d / % .3d. Start at % s ", expid, param.nExps, aggreItr + 1, param.AggreIter, ctime(&legacyStart));  // 將模型一回聚合回合開始時間輸出在終端機上				
				fprintf (f, "Experiment:Iter % .3d / % .3d : % .3d / % .3d. Start at % s ", expid, param.nExps, aggreItr + 1, param.AggreIter, ctime(&legacyStart));  // 將模型一回聚合回合開始時間輸出在檔案中
				
				// 模型一回聚合回合開始時間 (Eva_OneAggreTime 使用)
				auto startOneAggreTime = high_resolution_clock::now();


				#ifdef _WIN64 // Windows x64 環境下使用

					std::cout << "\t 電腦端遇到 紀錄功耗占用或記憶體占用 直接跳過" << endl;
				#else// Linux 環境下使用
					// 子行程 // 一次只能測一組 POWER 或 MEMORY ( pythonRunner_client只能在 linux 環境下跑)
					if (SwitchSegment.compare("ALL") == 0)
					{
						if (SwitchEvaluation.compare("POWER") == 0)  // EVALPOWER
						{
							//Power        "/home/raspdslab/evaPoolPath/logOneAggrePower.txt"
							if (SwitchDevice.compare("RASP") == 0)
							{
								auto status = pythonRunner_client(evaPoolPath + string("INA219.py")          , evaPoolPath + string("logOneAggrePower.txt"));
								int statusCode = get<0>(status);
								cout << statusCode << endl;
							}
							else if (SwitchDevice.compare("NANO") == 0)
							{
								auto status = pythonRunner_client(evaPoolPath + string("logOneAggrePower.py"), evaPoolPath + string("logOneAggrePower.txt"));
								int statusCode = get<0>(status);
								cout << statusCode << endl;
							}

						}
						else if ((SwitchEvaluation.compare("MEMORY") == 0))   // EVALPOWER
						{
							//Memory
							if (SwitchDevice.compare("RASP") == 0)
							{
								auto status = pythonRunner_client(evaPoolPath + string("logOneAggreMemory.py"), evaPoolPath + string("logOneAggreMemory.txt")); 
								int statusCode = get<0>(status);
								cout << statusCode << endl;
							}
							else if (SwitchDevice.compare("NANO") == 0)
							{
								auto status = pythonRunner_client(evaPoolPath + string("logOneAggreMemory.py"), evaPoolPath + string("logOneAggreMemory.txt")); 
								int statusCode = get<0>(status);
								cout << statusCode << endl;
							}
						}
					}
				#endif
				
				
				
				string str_EXP = to_string(expid);
				if (aggreItr < param.AggreIter - 1)				// 模型微調時使用
				{
					XtranPath = LoadDataPath + str_EXP + INIT_CLIENT_XtranMtxTxt;
					XtestPath = LoadDataPath + str_EXP + INIT_CLIENT_XtuneMtxTxt;
					LtranPath = LoadDataPath + str_EXP + INIT_CLIENT_LtranMtxTxt;  //Label;
					LtestPath = LoadDataPath + str_EXP + INIT_CLIENT_LtuneMtxTxt;
				}
				else				                            // 模型測試時使用
				{
					XtranPath = LoadDataPath + str_EXP + INIT_CLIENT_XtranMtxTxt;
					XtestPath = LoadDataPath + str_EXP + INIT_CLIENT_XtestMtxTxt;
					LtranPath = LoadDataPath + str_EXP + INIT_CLIENT_LtranMtxTxt;
					LtestPath = LoadDataPath + str_EXP + INIT_CLIENT_LtestMtxTxt;
				}
				LabelCountPath = LoadDataPath + str_EXP + INIT_CLIENT_LabelCountMtxTxt;
				auto GetCurrentTimeStampData1 = GetCurrentTimeStamp_client(3);                     // 獲取當時時間點

				//LAllPath = LoadDataPath + str_EXP + INIT_CLIENT_LtestMtxTxt;



				// 下面的 while 迴圈是等待 SERVER 端傳送資料集和標籤集到 NODE 端
				while (((get<0>(FileExists_client(XtranPath))) && (get<0>(FileExists_client(XtestPath))) && (get<0>(FileExists_client(XtranPath))) && (get<0>(FileExists_client(LtestPath))) && (get<0>(FileExists_client(LabelCountPath)))) == false)
				{
					cout << "\t\t 第" + str_EXP + "次更新\t";
					printf("Wait for the server sending Xtran...\n");
					// 每 1 秒偵測一次檔案是否存在
					auto FileExistsTime      = steady_clock::now();
					auto sleepFileExistsTime = FileExistsTime + milliseconds(1000);
					std::this_thread::sleep_until(sleepFileExistsTime);
				}

				//Input Assign Data 將server端傳過來的資料集讀取進來 
				MatrixXd* Xtran                = new MatrixXd(std::get<0>(LoadData_client(XtranPath)));
				MatrixXd* Xtest                = new MatrixXd(std::get<0>(LoadData_client(XtestPath)));
				MatrixXi* ttranLabelEnc        = new MatrixXi(std::get<0>(LoadData_client(LtranPath)).cast<int>()); // 直接在這裡進行型態轉換 
				MatrixXi* ttestLabelEnc        = new MatrixXi(std::get<0>(LoadData_client(LtestPath)).cast<int>());
				//Get Anchor
				auto AnchorData                = getAnchor_client(Xtran, param.J);
				MatrixXd* mid_anchors          = std::get<0>(AnchorData);
				MatrixXd* anchors              = new MatrixXd((*mid_anchors).transpose());
				MatrixXd* anchidx              = std::get<1>(AnchorData);

				// 抓取該回訓練及測試資料有多少種類別
				int       LabelCount           = std::get<0>(LoadData_client(LabelCountPath))(0, 0);

				MatrixXi All_Label((*ttranLabelEnc).rows() + (*ttestLabelEnc).rows(), (*ttranLabelEnc).cols());
				// 合併 ttranLabelEnc, ttestLabelEnc
				All_Label << *ttranLabelEnc, *ttestLabelEnc;
				auto uniqLabelsData            = uniqLabels_client(&All_Label);
				vector<int> uniqLabelList      = std::get<0>(uniqLabelsData);
				vector<int>::iterator ip       = std::get<1>(uniqLabelsData);



				// 生成 ttestMultHot  uniqLabelList.size()
				MatrixXi* ttestMultHot = new MatrixXi(MatrixXi::Zero(LabelCount, Xtest->cols()));
				//int first_Label              = uniqLabelList[0];
				for (int numY = 0; numY < Xtest->cols(); numY++)
				{
					(*ttestMultHot)((*ttestLabelEnc)(numY, 0) - 1, numY) = 1;
				}
				
				
				
////==========================================================訓練與測試
////==========================================================
////==========================================================				
				printf("\t Start training %s\n", SwitchMethod.c_str());

				// SDHWrapper_client 含9個欄位，傳回值意義請參考SDHWrapper_client
                auto SDHWrapperOutput = SDHWrapper_client(Xtran          ,          Xtest, ttranLabelEnc    , ttestLabelEnc, anchors      , param.L         , param.LocalIteration, 
				                                          SDH_K.Function , SDH_K.RBFsigma, SDH_K.HammingDist, SDH_K.ProxMu , SDH_K.FmapNu , SDH_K.GmapLambda, SDH_K.FmapLambda    ,
				                                          expid, aggreItr, Bpre          , SwitchDevice     , SwitchMethod , SwitchSegment, SwitchEvaluation, 
														  LoadDataPath   , evaPoolPath   , LabelCount);
				// Return SDHWrapper Data
				MatrixXi* Rpred_SDH          = get<0>(SDHWrapperOutput);            // 預測結果矩陣
				MatrixXd* hamDist_SDH        = get<1>(SDHWrapperOutput);            // 漢明距離矩陣
				Bpre                         = get<2>(SDHWrapperOutput);            // 前一聚合輪次的 B 矩陣
				
				
				
////==========================================================時間 測試耗能 測試占用記憶體
////==========================================================
////==========================================================

				#ifdef _WIN64 // Windows x64 環境下使用
					high_resolution_clock::time_point startTranTime             = get<3>(SDHWrapperOutput);   // 紀錄模型訓練開始時間戳
					high_resolution_clock::time_point endTranTime               = get<4>(SDHWrapperOutput);   // 紀錄模型訓練結束時間戳
					high_resolution_clock::time_point startTestTime             = get<5>(SDHWrapperOutput);   // 紀錄模型測試開始時間戳		
					high_resolution_clock::time_point endTestTime               = get<6>(SDHWrapperOutput);				
					high_resolution_clock::time_point startWaitAggreWeightTime  = get<7>(SDHWrapperOutput);   // 紀錄等待 Server 傳回聚合權重開始時間戳
					high_resolution_clock::time_point endWaitAggreWeightTime    = get<8>(SDHWrapperOutput);   // 紀錄等待 Server 傳回聚合權重結束時間戳
				
					// 計算測試時間
					duration<double, std::milli>      TestTime                  = endTestTime  - startTestTime;
					// 計算訓練時間
					duration<double, std::milli>      TranTime                  = endTranTime  - startTranTime    - (endWaitAggreWeightTime - startWaitAggreWeightTime);
					// 計算一回聚合輪次結束時間，並扣除等待 Server 傳回聚合權重的時間 
					duration<double, std::milli>      OneAggreTime              = endTestTime  - startOneAggreTime - (endWaitAggreWeightTime - startWaitAggreWeightTime); 
				
					std::cout << "\t\t The elapsed time for training        is " << TranTime.count()     << " ms " << endl;				
					std::cout << "\t\t The elapsed time for testing         is " << TestTime.count()     << " ms " << endl;
					std::cout << "\t\t The elapsed time for one aggregation is " << OneAggreTime.count() << " ms " << endl;
					
				#else// Linux 環境下使用
					// 子行程 // 砍掉 Test 和 OneAggre 的子行程 (紀錄功耗或占用記憶體的子行程)
					if (SwitchSegment.compare("TRAINTEST") == 0)                     // TEST
					{
						if (SwitchEvaluation.compare("POWER") == 0)                  // Power
						{
							if (SwitchDevice.compare("RASP") == 0)               // RASP
							{
								system(("pkill -f " + evaPoolPath + string("INA219.py")).c_str());               // 砍掉子行程
							}
							else if (SwitchDevice.compare("NANO") == 0)          // NANO
							{
								system(("pkill -f " + evaPoolPath + string("logTestPower.py")).c_str());
							}
							calcPython        = "python3 " + evaPoolPath + "calcTestPower.py";
                            calcSavePath      = evaPoolPath + "logTestPower.txt";
							calcPythonCommand = calcPython + " " + calcSavePath;                   // 呼叫 Python 檔案，並傳遞參數
						}
						else if ((SwitchEvaluation.compare("MEMORY") == 0))          // Memory
						{
							if (SwitchDevice.compare("RASP") == 0)               // RASP
							{
								system(("pkill -f " + evaPoolPath + string("logTestMemory.py")).c_str());
							}
							else if (SwitchDevice.compare("NANO") == 0)          // NANO
							{
								system(("pkill -f " + evaPoolPath + string("logTestMemory.py")).c_str());
							}
							calcPython        = "python3 " + evaPoolPath + "calcTestMemory.py";
                            calcSavePath      = evaPoolPath + "logTestMemory.txt";
							calcPythonCommand = calcPython + " " + calcSavePath;                   // 呼叫 Python 檔案，並傳遞參數
						}
					}
					else if (SwitchSegment.compare("ALL") == 0)
					{
						if (SwitchEvaluation.compare("POWER") == 0)     // Power
						{
							if (SwitchDevice.compare("RASP") == 0)               // RASP
							{
								system(("pkill -f " + evaPoolPath + string("INA219.py")).c_str());
							}
							else if (SwitchDevice.compare("NANO") == 0)          // NANO
							{
								system(("pkill -f " + evaPoolPath + string("logOneAggrePower.py")).c_str());
							}
							calcPython        = "python3 " + evaPoolPath + "calcOneAggrePower.py";
                            calcSavePath      = evaPoolPath + "logOneAggrePower.txt";
							calcPythonCommand = calcPython + " " + calcSavePath;                   // 呼叫 Python 檔案，並傳遞參數
						}
						else if ((SwitchEvaluation.compare("MEMORY") == 0))    // Memory
						{
							if (SwitchDevice.compare("RASP") == 0)               // RASP
							{
								system(("pkill -f " + evaPoolPath + string("logOneAggreMemory.py")).c_str());
							}
							else if (SwitchDevice.compare("NANO") == 0)          // NANO
							{
								system(("pkill -f " + evaPoolPath + string("logOneAggreMemory.py")).c_str());
							}
							calcPython        = "python3 " + evaPoolPath + "calcOneAggreMemory.py";
                            calcSavePath      = evaPoolPath + "logOneAggreMemory.txt";
							calcPythonCommand = calcPython + " " + calcSavePath;                   // 呼叫 Python 檔案，並傳遞參數
						}
					}
					wait(nullptr); // 等待子行程執行完畢
					std::cout << "Parent process is done." << std::endl;
					FILE* pipe = popen(calcPythonCommand.c_str(), "r");          	// 開啟一個管道並執行計算 python 檔 
					// 檢查管道是否成功開啟
					if (!pipe) {
						cerr << "popen() failed!" << endl;
						exit(1);
					}
					// 從管道中讀取資料並儲存到字串中
					char buffer[128];
					string result = "";
					while (!feof(pipe)) {
						if (fgets(buffer, 128, pipe) != NULL) {
							result += buffer;
						}
					}
					pclose(pipe);
					// 印出Python程式執行後的結果
					cout << result << endl;
				#endif				
				
				
				
				
////==========================================================評估指標
////==========================================================
////==========================================================				
				

				// 將預測結果矩陣轉換為 Label 
				// getLabel_client 含三個欄位，傳回值意義請參考 下 3 行
				auto     getLabelData        = getLabel_client(Rpred_SDH, ttranLabelEnc, LabelCount);
				MatrixXi tpredMultHotByThres = get<0>(getLabelData).cwiseMax(0).array().sign().matrix().cast<int>();   //由 votetable 前 K 大值來製作多熱編碼或獨熱編碼 
				MatrixXi tpredMultHot        = get<1>(getLabelData);               //是由{0,1}組成，表示該column的測試資料，對應的row標籤結果
				MatrixXi tpredLabEnc         = get<2>(getLabelData);               //測試資料的標籤結果label encoding

 

				//取Acc, Pre, Rec,F1
				auto evaluateData  = evaluate_client(*ttestMultHot, tpredMultHotByThres, switchEval_METHOD);
				double acc         = get<0>(evaluateData);
				double pre         = get<1>(evaluateData);
				double rec         = get<2>(evaluateData);
				double f1          = get<3>(evaluateData);
				//取mAP
				auto mAPData       = multilabelmaPrecision_client(*ttestMultHot, tpredMultHotByThres); 
				double mAP         = get<0>(mAPData);
				
				
				Eva_acc.push_back(acc); Eva_pre.push_back(pre); Eva_rec.push_back(rec);
				Eva_f1.push_back(f1)  ; Eva_mAP.push_back(mAP); Eva_TranTime.push_back(TranTime.count());
				Eva_TestTime.push_back(TestTime.count())      ; Eva_OneAggreTime.push_back(OneAggreTime.count());				
				
				// 記錄每一回局部更新權重的評估指標
				printf(    "\t acc:\t%3.3f, pre:\t%3.3f, rec:\t%3.3f, f1 :\t%3.3f, mAP:\t%3.3f\n", acc, pre, rec, f1, mAP);// 將模型一回聚合回合結束時間輸出在終端機上
				fprintf(f, "\t acc:\t%3.3f, pre:\t%3.3f, rec:\t%3.3f, f1 :\t%3.3f, mAP:\t%3.3f\n", acc, pre, rec, f1, mAP);// 將模型一回聚合回合結束時間輸出在檔案中


////==========================================================
////==========================================================
////==========================================================
				// 印出最後一回模型更新結束時間
				if ((expid * param.AggreIter) + aggreItr + 1 == param.nExps * param.AggreIter)
				{
					auto endAggreItr = system_clock::now();
					auto legacyEnds  = system_clock::to_time_t(endAggreItr);
					printf (    "Experiment:Iter % .3d / % .3d : % .3d / % .3d.  Ends at % s \n", expid, param.nExps, aggreItr + 1, param.AggreIter, ctime(&legacyEnds));// 將模型最後一回結束時間輸出在終端機上
					fprintf(f,  "Experiment:Iter % .3d / % .3d : % .3d / % .3d.  Ends at % s \n", expid, param.nExps, aggreItr + 1, param.AggreIter, ctime(&legacyEnds));// 將模型最後一回結束時間輸出在檔案中					
				}
				if ((datasetnam.compare("SMALL_ARFACE") == 0) && (aggreItr > (param.AggreIter - 2))) // 等待 Server 端產生完資料，不然有時會生成只有名字沒有資料的txt檔
				{
					auto waitServerDeleteFileTime  = steady_clock::now();
					auto sleepServerDeleteFileTime = waitServerDeleteFileTime + milliseconds(23000);
					std::this_thread::sleep_until(sleepServerDeleteFileTime);
					auto GetCurrentTimeStampData6  = GetCurrentTimeStamp_client(3);                  // 獲取當時時間點
					std::cout << get<0>(GetCurrentTimeStampData6) << endl;
				}
				
				
				
				
				
				
				
				
				

			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		// 每一輪聚合輪次後的評估指標(包含Tunning的輪次)
		printf (    "TUNNING_OneAggreEvaluate\n");
		fprintf(f, "TUNNING_OneAggreEvaluate\n");
		auto   Eva_accData  = meanstd_client(Eva_acc);
		double Eva_accMean  = get<0>(Eva_accData);
		double Eva_accStd   = get<1>(Eva_accData);
		printf(    "%s ACCURACY            mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_accMean, Eva_accStd);
		fprintf(f, "%s ACCURACY            mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_accMean, Eva_accStd);
		auto   Eva_preData  = meanstd_client(Eva_pre);
		double Eva_preMean  = get<0>(Eva_preData);
		double Eva_preStd   = get<1>(Eva_preData);
		printf(    "%s PRECISION           mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_preMean, Eva_preStd);
		fprintf(f, "%s PRECISION           mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_preMean, Eva_preStd);
		auto   Eva_recData  = meanstd_client(Eva_rec);
		double Eva_recMean  = get<0>(Eva_recData);
		double Eva_recStd   = get<1>(Eva_recData);
		printf(    "%s RECALL              mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_recMean, Eva_recStd);
		fprintf(f, "%s RECALL              mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_recMean, Eva_recStd);
		auto   Eva_f1Data   = meanstd_client(Eva_f1);
		double Eva_f1Mean   = get<0>(Eva_f1Data);
		double Eva_f1Std    = get<1>(Eva_f1Data);
		printf(    "%s F1                  mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_f1Mean , Eva_f1Std );
		fprintf(f, "%s F1                  mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_f1Mean , Eva_f1Std );
		auto   Eva_mAPData  = meanstd_client(Eva_mAP);
		double Eva_mAPMean  = get<0>(Eva_mAPData);
		double Eva_mAPStd   = get<1>(Eva_mAPData);
		printf(    "%s mAP                 mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_mAPMean, Eva_mAPStd);
		fprintf(f, "%s mAP                 mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_mAPMean, Eva_mAPStd);
		auto   Eva_TranTimeData        = meanstd_client(Eva_TranTime);
		double Eva_TranTimeMean        = get<0>(Eva_TranTimeData);
		double Eva_TranTimeStd         = get<1>(Eva_TranTimeData);
		printf(    "%s TranTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TranTimeMean, Eva_TranTimeStd);
		fprintf(f, "%s TranTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TranTimeMean, Eva_TranTimeStd);
		auto   Eva_TestTimeData        = meanstd_client(Eva_TestTime);
		double Eva_TestTimeMean        = get<0>(Eva_TestTimeData);
		double Eva_TestTimeStd         = get<1>(Eva_TestTimeData);
		printf(    "%s TestTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TestTimeMean, Eva_TestTimeStd);
		fprintf(f, "%s TestTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TestTimeMean, Eva_TestTimeStd);
		auto   Eva_OneAggreTimeData    = meanstd_client(Eva_OneAggreTime);
		double Eva_OneAggreTimeMean    = get<0>(Eva_OneAggreTimeData);
		double Eva_OneAggreTimeStd     = get<1>(Eva_OneAggreTimeData);
		printf(    "%s OneAggreTime (ms)   mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_OneAggreTimeMean, Eva_OneAggreTimeStd);
		fprintf(f, "%s OneAggreTime (ms)   mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_OneAggreTimeMean, Eva_OneAggreTimeStd);
		//Tunning完後的評估指標(不包含 Tunning 的回數，只計算最後一輪使用 TestData下的結果)
		printf(    "TESTING_Evaluate\n");
		fprintf(f, "TESTING_Evaluate\n");
		auto   Eva_accDataPerAggreIter  = meanstdPerAggreIter_client(Eva_acc, param.AggreIter);            // ### Eva_accData_10 => Eva_accDataPerAggreIter
		double Eva_accMeanPerAggreIter  = get<0>(Eva_accDataPerAggreIter);
		double Eva_accStdPerAggreIter   = get<1>(Eva_accDataPerAggreIter);
		printf(    "%s ACCURACY            mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_accMeanPerAggreIter, Eva_accStdPerAggreIter);
		fprintf(f, "%s ACCURACY            mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_accMeanPerAggreIter, Eva_accStdPerAggreIter);
		auto   Eva_preDataPerAggreIter  = meanstdPerAggreIter_client(Eva_pre, param.AggreIter);
		double Eva_preMeanPerAggreIter  = get<0>(Eva_preDataPerAggreIter);
		double Eva_preStdPerAggreIter   = get<1>(Eva_preDataPerAggreIter);
		printf(    "%s PRECISION           mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_preMeanPerAggreIter, Eva_preStdPerAggreIter);
		fprintf(f, "%s PRECISION           mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_preMeanPerAggreIter, Eva_preStdPerAggreIter);
		auto   Eva_recDataPerAggreIter  = meanstdPerAggreIter_client(Eva_rec, param.AggreIter);
		double Eva_recMeanPerAggreIter  = get<0>(Eva_recDataPerAggreIter);
		double Eva_recStdPerAggreIter   = get<1>(Eva_recDataPerAggreIter);
		printf(    "%s RECALL              mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_recMeanPerAggreIter, Eva_recStdPerAggreIter);
		fprintf(f, "%s RECALL              mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_recMeanPerAggreIter, Eva_recStdPerAggreIter);
		auto   Eva_f1DataPerAggreIter   = meanstdPerAggreIter_client(Eva_f1, param.AggreIter);
		double Eva_f1MeanPerAggreIter   = get<0>(Eva_f1DataPerAggreIter);
		double Eva_f1StdPerAggreIter    = get<1>(Eva_f1DataPerAggreIter);
		printf(    "%s F1                  mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_f1MeanPerAggreIter , Eva_f1StdPerAggreIter );
		fprintf(f, "%s F1                  mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_f1MeanPerAggreIter , Eva_f1StdPerAggreIter );
		auto   Eva_mAPDataPerAggreIter  = meanstdPerAggreIter_client(Eva_mAP, param.AggreIter);
		double Eva_mAPMeanPerAggreIter  = get<0>(Eva_mAPDataPerAggreIter);
		double Eva_mAPStdPerAggreIter   = get<1>(Eva_mAPDataPerAggreIter);
		printf(    "%s mAP                 mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_mAPMeanPerAggreIter, Eva_mAPStdPerAggreIter);
		fprintf(f, "%s mAP                 mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_mAPMeanPerAggreIter, Eva_mAPStdPerAggreIter);
		auto   Eva_TranTimeDataPerAggreIter  = meanstdPerAggreIter_client(Eva_TranTime, param.AggreIter);
		double Eva_TranTimeMeanPerAggreIter  = get<0>(Eva_TranTimeDataPerAggreIter);
		double Eva_TranTimeStdPerAggreIter   = get<1>(Eva_TranTimeDataPerAggreIter);
		printf(    "%s TranTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TranTimeMeanPerAggreIter, Eva_TranTimeStdPerAggreIter);
		fprintf(f, "%s TranTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TranTimeMeanPerAggreIter, Eva_TranTimeStdPerAggreIter);
		auto   Eva_TestTimeDataPerAggreIter   = meanstdPerAggreIter_client(Eva_TestTime, param.AggreIter);
		double Eva_TestTimeMeanPerAggreIter   = get<0>(Eva_TestTimeDataPerAggreIter);
		double Eva_TestTimeStdPerAggreIter    = get<1>(Eva_TestTimeDataPerAggreIter);
		printf(    "%s TestTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TestTimeMeanPerAggreIter, Eva_TestTimeStdPerAggreIter);
		fprintf(f, "%s TestTime (ms)       mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_TestTimeMeanPerAggreIter, Eva_TestTimeStdPerAggreIter);
		auto   Eva_OneAggreTimeDataPerAggreIter    = meanstdPerAggreIter_client(Eva_OneAggreTime, param.AggreIter);
		double Eva_OneAggreTimeMeanPerAggreIter    = get<0>(Eva_OneAggreTimeDataPerAggreIter);
		double Eva_OneAggreTimeStdPerAggreIter     = get<1>(Eva_OneAggreTimeDataPerAggreIter);
		printf(    "%s OneAggreTime (ms)   mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_OneAggreTimeMeanPerAggreIter, Eva_OneAggreTimeStdPerAggreIter);
		fprintf(f, "%s OneAggreTime (ms)   mean : %.7f  std : %.7f \n", SwitchMethod.c_str(), Eva_OneAggreTimeMeanPerAggreIter, Eva_OneAggreTimeStdPerAggreIter); 

		printf(    "DataName        :\t\t % s\n" , datasetnam.c_str()   );
		printf(    "SwitchMethod    :\t\t % s\n",  SwitchMethod.c_str() );
		printf(    "nExps           :\t\t % 3d\n", param.nExps          );
		printf(    "L               :\t\t % 3d\n", param.L              );
		printf(    "AggreIter       :\t\t % 3d\n", param.AggreIter      );
		printf(    "LocalIteration  :\t\t % 3d\n", param.LocalIteration );
		printf(    "RBFSIGMA        :\t\t % lf\n", SDH_K.RBFsigma       );
		printf(    "HAMMRADIUS      :\t\t % 3d\n", SDH_K.HammingDist    );
		printf(    "ProxMu          :\t\t % s\n" , ProxMu.str().c_str() );

		fprintf(f, "DataName        :\t\t % s\n" , datasetnam.c_str()   );
		fprintf(f, "SwitchMethod    :\t\t % s\n",  SwitchMethod.c_str() );
		fprintf(f, "nExps           :\t\t % 3d\n", param.nExps          );
		fprintf(f, "L               :\t\t % 3d\n", param.L              );
		fprintf(f, "AggreIter       :\t\t % 3d\n", param.AggreIter      );
		fprintf(f, "LocalIteration  :\t\t % 3d\n", param.LocalIteration );
		fprintf(f, "RBFSIGMA        :\t\t % lf\n", SDH_K.RBFsigma       );
		fprintf(f, "HAMMRADIUS      :\t\t % 3d\n", SDH_K.HammingDist    );
		fprintf(f, "ProxMu          :\t\t % s\n" , ProxMu.str().c_str() );

		// 所有程式結束已用時間
		auto endElapsedTime                      = high_resolution_clock::now();
		duration<double, std::milli> ElapsedTime = endElapsedTime - startElapsedTime;
		std::cout << "The elapsed time for Experiment is " << ElapsedTime.count()                                   << " ms " << endl;// 所有程式執行結束時間
		std::cout << "The one time for Experiment is "     << ElapsedTime.count() / (param.nExps * param.AggreIter) << " ms " << endl;// 一回程式執行結束時間


		fclose(f); // 關閉文件      
		//恢復cout原有的缓冲區指针
		//std::cout.rdbuf(coutbuf);

		auto GetCurrentTimeStampData2 = GetCurrentTimeStamp_client(3);                      // 獲取當時時間點

        if (testBitIdx < (bitlen.size() - 1))
        {
            auto GetCurrentTimeStampData4 = GetCurrentTimeStamp_client(3);                  // 獲取當時時間點
            std::cout << get<0>(GetCurrentTimeStampData4) << endl;
            EmptyWeightPool_client(weightPoolPath);                                         // 查詢此資料夾是否清空 (EmptyWeightPool_client 只能在 linux 環境下跑)
            RemoveEvaLogFile_client(evaPoolPath, SwitchSegment, SwitchEvaluation);          // 刪除前一個測試碼長留下的 Power 和 Memory 的 紀錄檔案 (logOneAggrePower.txt , logTrainPower.txt, logTestPower.txt, logOneAggreMemory.txt , logTrainMemory.txt, logTestMemory.txt)
        }
        std::cout << get<0>(GetCurrentTimeStampData2) << endl;
        // 休息五秒
        auto waitTime  = steady_clock::now();
        auto sleepTime = waitTime + milliseconds(8000);
        std::this_thread::sleep_until(sleepTime);
        auto GetCurrentTimeStampData3 = GetCurrentTimeStamp_client(3);                      // 獲取當時時間點
        std::cout << get<0>(GetCurrentTimeStampData3) << endl;
		if ((datasetnam.compare("SMALL_ARFACE") == 0))                                      // 等待 Server 端刪除完共享資料夾的東西不然會出錯
		{
			auto waitServerDeleteFileTime  = steady_clock::now();
			auto sleepServerDeleteFileTime = waitServerDeleteFileTime + milliseconds(38000);
			std::this_thread::sleep_until(sleepServerDeleteFileTime);
			auto GetCurrentTimeStampData5  = GetCurrentTimeStamp_client(3);                  // 獲取當時時間點
			std::cout << get<0>(GetCurrentTimeStampData5) << endl;
		}
        else if ((datasetnam.compare("YALE") == 0))                                      // 等待 Server 端刪除完共享資料夾的東西不然會出錯
        {
            auto waitServerDeleteFileTime  = steady_clock::now();
            auto sleepServerDeleteFileTime = waitServerDeleteFileTime + milliseconds(13000);
            std::this_thread::sleep_until(sleepServerDeleteFileTime);
            auto GetCurrentTimeStampData5  = GetCurrentTimeStamp_client(3);                  // 獲取當時時間點
            std::cout << get<0>(GetCurrentTimeStampData5) << endl;
        }
        else if ((datasetnam.compare("JAFFE") == 0))                                      // 等待 Server 端刪除完共享資料夾的東西不然會出錯
        {
            auto waitServerDeleteFileTime  = steady_clock::now();
            auto sleepServerDeleteFileTime = waitServerDeleteFileTime + milliseconds(13000);
            std::this_thread::sleep_until(sleepServerDeleteFileTime);
            auto GetCurrentTimeStampData5  = GetCurrentTimeStamp_client(3);                  // 獲取當時時間點
            std::cout << get<0>(GetCurrentTimeStampData5) << endl;
        }
        else if ((datasetnam.compare("ORL") == 0))                                      // 等待 Server 端刪除完共享資料夾的東西不然會出錯
        {
            auto waitServerDeleteFileTime  = steady_clock::now();
            auto sleepServerDeleteFileTime = waitServerDeleteFileTime + milliseconds(23000);
            std::this_thread::sleep_until(sleepServerDeleteFileTime);
            auto GetCurrentTimeStampData5  = GetCurrentTimeStamp_client(3);                  // 獲取當時時間點
            std::cout << get<0>(GetCurrentTimeStampData5) << endl;
        }
//        string ClientEnd = INIT_CLIENT_ControlClientEnd;
//        std::string CtrlPath = INIT_CLIENT_ControlFolderPath;
//        std::string RASPFile = INIT_CLIENT_ControlSERVERRASPFile;
//        std::string CtrlRASPFile = CtrlPath + RASPFile;

//        std::ofstream Startfile(CtrlRASPFile);  // 調解SERVER 和 CLIENT 清空資料夾的時機
//        if (Startfile.is_open()) {  // 確保文件成功打開
//            Startfile << ClientEnd << std::endl;
//            Startfile.close();  // 關閉文件
//            cout << "字串已成功寫入 ControlSERVERNANOFile。" << std::endl;
//        }
//        else {
//            cout << "無法打開 ControlSERVERNANOFile。" << std::endl;
//        }

	}

	return 0;
}

