/*
@author:     Yu-Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo-Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    
Log: 0.0.1 Initial
     0.0.2 改成指標形式
	 0.0.3 sigma和Gmap_Lambda改double [BWC]
	       增加引數 double SDH_FmapNu  ,   double  SDH_GmapLambda,    double  SDH_FmapLambda 
     0.0.4 Training time display is moved to main.cpp [BWC]
	       The return adds endTestTime
	 
	 
	 
    在 Client 端訓練模型前得到更新模型所需的變數值( W, P 和 B 矩陣 )，執行模型的訓練和測試，訓練區段的模型功耗和記憶體使用率

TRAIN - Segment

    RASP - Power
    /home/raspdslab/evaPoolPath/INA219.py                     // 主程式 - 取用 Power 數據
    /home/raspdslab/evaPoolPath/logTrainPower.txt             // 印出掃描 Power 數據 txt 檔
    /home/raspdslab/evaPoolPath/calcTrainPower.py             // 計算 Power 數據 程式
    /home/raspdslab/evaPoolPath/calcTrainPower.txt            // 印出總和 Power 相關數據(平均功耗) txt 檔

    NANO - Power
    /home/bn10513137/evaPoolPath/logTrainPower.py             // 主程式 - 取用 Power 數據
    /home/bn10513137/evaPoolPath/logTrainPower.txt            // 印出掃描 Power 數據 txt 檔
    /home/bn10513137/evaPoolPath/calcTrainPower.py            // 計算 Power 數據 程式
    /home/bn10513137/evaPoolPath/calcTrainPower.txt           // 印出總和 Power 相關數據(平均功耗) txt 檔

    RASP - Memory
    /home/raspdslab/evaPoolPath/logTrainMemory.py             // 主程式 - 取用 Memory 數據
    /home/raspdslab/evaPoolPath/logTrainMemory.txt            // 印出掃描 Memory 數據 txt 檔
    /home/raspdslab/evaPoolPath/calcTrainMemory.py            // 計算 Memory 數據 程式
    /home/raspdslab/evaPoolPath/calcTrainMemory.txt           // 印出總和 Memory 相關數據(平均功耗) txt 檔

    NANO - Memory
    /home/bn10513137/evaPoolPath/logTrainMemory.py            // 主程式 - 取用 Memory 數據
    /home/bn10513137/evaPoolPath/logTrainMemory.txt           // 印出掃描 Memory 數據 txt 檔
    /home/bn10513137/evaPoolPath/calcTrainMemory.py           // 計算 Memory 數據 程式
    /home/bn10513137/evaPoolPath/calcTrainMemory.txt          // 印出總和 Memory 相關數據(平均功耗) txt 檔

TEST - Segment

    RASP - Power
    /home/raspdslab/evaPoolPath/INA219.py                     // 主程式 - 取用 Power 數據
    /home/raspdslab/evaPoolPath/logTestPower.txt              // 印出掃描 Power 數據 txt 檔
    /home/raspdslab/evaPoolPath/calcTestPower.py              // 計算 Power 數據 程式
    /home/raspdslab/evaPoolPath/calcTestPower.txt             // 印出總和 Power 相關數據(平均功耗) txt 檔

    NANO - Power
    /home/bn10513137/evaPoolPath/logTestPower.py              // 主程式 - 取用 Power 數據
    /home/raspdslab/evaPoolPath/logTestPower.txt              // 印出掃描 Power 數據 txt 檔
    /home/bn10513137/evaPoolPath/calcTestPower.py             // 計算 Power 數據 程式
    /home/bn10513137/evaPoolPath/calcTestPower.txt            // 印出總和 Power 相關數據(平均功耗) txt 檔

    RASP - Memory
    /home/raspdslab/evaPoolPath/logTestMemory.py              // 主程式 - 取用 Memory 數據
    /home/raspdslab/evaPoolPath/logTestMemory.txt             // 印出掃描 Memory 數據 txt 檔
    /home/raspdslab/evaPoolPath/calcTestMemory.py             // 計算 Memory 數據 程式
    /home/raspdslab/evaPoolPath/calcTrainMemory.txt           // 印出總和 Memory 相關數據(平均功耗) txt 檔

    NANO - Memory
    /home/bn10513137/evaPoolPath/logTestMemory.py             // 主程式 - 取用 Memory 數據
    /home/bn10513137/evaPoolPath/logTestMemory.txt            // 印出掃描 Memory 數據 txt 檔
    /home/bn10513137/evaPoolPath/calcTestMemory.py            // 計算 Memory 數據 程式
    /home/bn10513137/evaPoolPath/calcTestMemory.txt           // 印出總和 Memory 相關數據(平均功耗) txt 檔

    Input Parameters
        ----------
        Xtran            : array  ; D     - by - Ntran ; 訓練資料矩陣
        Xtest            : array  ; D     - by - Ntest ; 測試資料矩陣
        ttranLabelEnc    : vector ; Ntran - by - 1     ; 訓練資料標籤
        ttestLabelEnc    : vector ; Ntest - by - 1     ; 測試資料標籤
        Anchor           : array  ; D     - by - A     ; 錨點矩陣
        L                : int    ; Hash code長度
        LocalIteration   : int    ; 迭代次數
        kernel 參數
        SDH_Function     : str    ; kernel名稱
        SDH_RBFsigma     : double ; 參數sigma
        SDH_HammingDist  : double ; 漢明距離
        SDH_Mu           : double ; FedProx 正則化參數
        exp              : double ; 執行幾回完整的模型更新
        itr              : double ; 最大伺服端聚合輪次
        *Bpre            : array* ; Ntran - by - L     ; 前一聚合輪次的 B 矩陣
        SwitchDevice     : str    ; 選擇在哪個裝置上進行權重更新(NANO , RASP)
        SwitchMethod     : str    ; 選擇何種聯邦學習式模型(AVG_SDH, AVG_FSDH, PROX_SDH 和 PROX_FSDH)
        SwitchSegment    : str    ; 選擇量測完整一回模型更新區段或是在模型訓練或模型測試區段進行(ALL 和 TRAINTEST)
        SwitchEvaluation : str    ; 選擇量測何種評估指標  (SPEED, POWER 和 MEMORY)
        LoadDataPath     : str    ; LoadDataPath = "..\\weightPoolPath\\SERVER2NANO_"
        evaPoolPath      : str    ; 放置 POWER 或 MEMORY 相關資料
    Output Parameters
        ----------
        Rpred_SDH    : array*     ; Ntran - by - Ntest    ; 預測結果矩陣
        hamDist_SDH  : array*     ; Ntran - by - Ntest    ; 漢明距離矩陣
        *Bpre   : array*          ; Ntran - by - L        ; 前一聚合輪次的 B 矩陣
        startTest                 : high_resolution_clock ; 紀錄模型測試開始時間戳
        startTrain                : high_resolution_clock ; 紀錄模型訓練開始時間戳
        endTrain                  : high_resolution_clock ; 紀錄模型訓練結束時間戳
        start_WaitAggreWeight     : high_resolution_clock ; 紀錄等待 Server 傳回聚合權重開始時間戳
        end_WaitAggreWeight       : high_resolution_clock ; 紀錄等待 Server 傳回聚合權重結束時間戳

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
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <tuple>
#include <ctime>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "SDH_client.h"
#include "sqdist_client.h"
#include "SDHWrapper_client.h"
#include "compactbit_client.h"
#include "hammingDist_client.h"
#include "pythonRunner_client.h"
using namespace std;
using namespace Eigen;
using namespace cv;


//struct param_struct {
//    int    nExps;                                    // 執行幾回完整的模型更新
//    int    J;                                        // 錨點
//    int    L;                                        // 編碼長度
//    int    AggreIter;                                // 最大伺服端聚合輪次
//    int    LocalIteration;                           // 最大伺服端聚合輪次
//}param;

std::tuple<
             MatrixXi*                                     , MatrixXd*                                     , MatrixXd*                                     ,
			 std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point, 
			 std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point 
		  > 
SDHWrapper_client
          ( MatrixXd* Xtran    , MatrixXd* Xtest    , MatrixXi* ttranLabelEnc, MatrixXi* ttestLabelEnc, MatrixXd* Anchor   , 
		    int L              , int LocalIteration , 
		    string SDH_Function, double SDH_RBFsigma, int SDH_HammingDist    , double SDH_Mu          , double SDH_FmapNu  , double  SDH_GmapLambda, double  SDH_FmapLambda,   
		    int exp            , int itr            , MatrixXd* Bpre         , 
		    string SwitchDevice, string SwitchMethod, string SwitchSegment   , string SwitchEvaluation, string LoadDataPath, string evaPoolPath    , int LabelCount
		   )
{
    // 參數設定
    double    sigma        = SDH_RBFsigma;
    int       nbits        = L;                     // 程式中須測試的碼長，主要碼長設定在param.L
    string    Gmap_loss    = "L2";
    string    Fmap_type    = SDH_Function;
    double    Fmap_Nu      = SDH_FmapNu;//1e-5;     // 懲罰參數 Fmap_Nu * ||B - P'Phi||_2	        定義在main_client SDH_Kernel_struct
    double    Gmap_Lambda  = SDH_GmapLambda;//1;    // G-STEP Lambda   求WWeight的Ridge Param    定義在main_client SDH_Kernel_struct	
    double    Fmap_Lambda  = SDH_FmapLambda;//1e-2; // F-STEP Lambda   求PWeight的Ridge Param    定義在main_client SDH_Kernel_struct
	                                                // FedProx Mu                               定義在main_client SDH_Kernel_struct
    double    tol          = 1e-7;                  // bias 參數;
    MatrixXd* anchor       = Anchor;
    MatrixXd* traindata    = new MatrixXd(Xtran->transpose());
    MatrixXd* testdata     = new MatrixXd(Xtest->transpose());
    MatrixXi* traingnd     = ttranLabelEnc;
    MatrixXi* testgnd      = ttestLabelEnc;
    int       Ntran        = traindata->rows();

    string    calcPython, calcSavePath, calcPythonCommand;             // 計算 POWER 或 MEMORY 的 python 檔 的指令 ; 儲存 POWER 或 MEMORY 計算後相關數據的檔案路徑 ; 代入參數計算指令(calcPython + 參數)
    //生成Train phi
    double    Phi_div      = 1.0 / (2 * sigma * sigma);
    MatrixXd* PhiTrain     = new MatrixXd((-get<0>(sqdist_client(traindata, anchor))->array() * Phi_div).exp().matrix());
    

    // 亂數生成初始 B 矩陣
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-10000, 10000);
    MatrixXd  Zinit1 = Eigen::MatrixXd::Zero(Ntran, nbits).unaryExpr([&](double dummy) {return dis(gen); });
    MatrixXi* Zinit  = new MatrixXi(Zinit1.array().sign().matrix().cast<int>());
    int       debug  = 0;

    // 紀錄模型訓練時間開始計時
    auto startTrainTime = std::chrono::high_resolution_clock::now();

    #ifdef _WIN64 // Windows x64 環境下使用
        // 紀錄模型訓練時間開始計時
    #else// Linux 環境下使用
        // 子進程 // 一次只能測一組 POWER 或 MEMORY ( pythonRunner_client只能在 linux 環境下跑)
        if (SwitchSegment.compare("TRAINTEST") == 0)
        {
            if (SwitchEvaluation.compare("POWER") == 0)  // EVALPOWER
            {
                //Power
                if (SwitchDevice.compare("RASP") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("INA219.py"), evaPoolPath + string("logTrainPower.txt")); 
                    int statusCode = get<0>(status);
                    std::cout << statusCode << std::endl;
                }
                else if (SwitchDevice.compare("NANO") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("logTrainPower.py"), evaPoolPath + string("logTrainPower.txt")); 
                    int statusCode = get<0>(status);
                    std::cout << statusCode << std::endl;
                }

            }
            else if ((SwitchEvaluation.compare("MEMORY") == 0))   // EVALPMEMORY
            {
                //Memory
                if (SwitchDevice.compare("RASP") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("logTrainMemory.py"), evaPoolPath + string("logTrainMemory.txt"));
                    int statusCode = get<0>(status);
                    std::cout << statusCode << std::endl;
                }
                else if (SwitchDevice.compare("NANO") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("logTrainMemory.py"), evaPoolPath + string("logTrainMemory.txt")); 
                    int statusCode = get<0>(status);
                    std::cout << statusCode << std::endl;

                }
            }
        }
    #endif










    //迭代更新 W, P 和 B 矩陣
    auto SDH_data = SDH_client(PhiTrain, traingnd, Zinit, SDH_Mu, Gmap_Lambda, Gmap_loss, Fmap_type, Fmap_Nu, Fmap_Lambda, tol, LocalIteration, debug, exp, itr, Bpre, SwitchMethod, LoadDataPath, LabelCount);
    MatrixXd *F_W = get<1>(SDH_data);
    MatrixXi *H   = get<2>(SDH_data);
    Bpre     = new MatrixXd(H->cast<double>());
    std::chrono::high_resolution_clock::time_point start_WaitAggreWeightTime = get<3>(SDH_data);           // 紀錄等待 Server 傳回聚合權重開始時間
    std::chrono::high_resolution_clock::time_point end_WaitAggreWeightTime   = get<4>(SDH_data);           // 紀錄等待 Server 傳回聚合權重結束時間

    // EVALUATION
    int AsymDist = 0;
    if (AsymDist)
    {
        *H = *H ;
    }
    else
    {
        *H = ((*PhiTrain) * (*F_W)).array().sign().matrix().cast<int>();
        *H = (*H).cwiseMax(0);
    }
    auto B_data = compactbit_client(H);
    MatrixXi* B = get<0>(B_data);

    // 紀錄模型訓練時間結束計時
    auto endTrainTime = std::chrono::high_resolution_clock::now();




    #ifdef _WIN64 // Windows x64 環境下使用
		//std::chrono::duration<double, std::milli> TrainTime = endTrainTime - startTrainTime - (end_WaitAggreWeightTime - start_WaitAggreWeightTime);
        //std::cout << "\t\t The elapsed time for Training Time is " << TrainTime.count() << " ms " << endl;

    #else// Linux 環境下使用
        // 子進程
        if (SwitchSegment.compare("TRAINTEST") == 0)
        {
            if (SwitchEvaluation.compare("POWER") == 0)                       // Power
            {
                if (SwitchDevice.compare("RASP") == 0)                    // RASP
                {
                    system(("pkill -f " + evaPoolPath + string("INA219.py")).c_str());  // 砍掉子行程
                }
                else if (SwitchDevice.compare("NANO") == 0)               // NANO
                {
                    system(("pkill -f " + evaPoolPath + string("logTrainPower.py")).c_str());
                }
                calcPython = "python3 " + evaPoolPath + "calcTrainPower.py";
                calcSavePath = evaPoolPath + "logTrainPower.txt";
                calcPythonCommand = calcPython + " " + calcSavePath;                // 呼叫 Python 檔案，並傳遞參數
            }
            else if ((SwitchEvaluation.compare("MEMORY") == 0))               // Memory
            {
                if (SwitchDevice.compare("RASP") == 0)                    // RASP
                {
                    system(("pkill -f " + evaPoolPath + string("logTrainMemory.py")).c_str());
                }
                else if (SwitchDevice.compare("NANO") == 0)               // NANO
                {
                    system(("pkill -f " + evaPoolPath + string("logTrainMemory.py")).c_str());
                }
                calcPython = "python3 " + evaPoolPath + "calcTrainMemory.py";
                calcSavePath = evaPoolPath + "logTrainMemory.txt";
                calcPythonCommand = calcPython + " " + calcSavePath;                 // 呼叫 Python 檔案，並傳遞參數
            }
            wait(nullptr); // 等待子進程執行完畢
            std::cout << "Parent process is done." << std::endl;
            FILE* pipe = popen(calcPythonCommand.c_str(), "r");
            if (!pipe) {
                cerr << "popen() failed!" << endl;
                exit(1);
            }
            char buffer[128];
            string result = "";

            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != NULL) {
                    result += buffer;
                }
            }
            pclose(pipe);
            cout << result << endl;        // 印出Python程式執行後的結果
        }

        if (SwitchSegment.compare("TRAINTEST") == 0)
        {
            if (SwitchEvaluation.compare("POWER") == 0)  // EVALPOWER
            {
                //Power        "/home/raspdslab/evaPoolPath/ONE_TRAIN_power.txt"
                if (SwitchDevice.compare("RASP") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("INA219.py"), evaPoolPath + string("logTestPower.txt"));
                    int statusCode = get<0>(status);
                    cout << statusCode << endl;
                }
                else if (SwitchDevice.compare("NANO") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("logTestPower.py"), evaPoolPath + string("logTestPower.txt"));
                    int statusCode = get<0>(status);
                    cout << statusCode << endl;
                }
            }
            else if ((SwitchEvaluation.compare("MEMORY") == 0))   // EVALPMEMORY
            {
                //Memory
                if (SwitchDevice.compare("RASP") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("logTestMemory.py"), evaPoolPath + string("logTestMemory.txt"));
                    int statusCode = get<0>(status);
                    cout << statusCode << endl;
                }
                else if (SwitchDevice.compare("NANO") == 0)
                {
                    auto status = pythonRunner_client(evaPoolPath + string("logTestMemory.py"), evaPoolPath + string("logTestMemory.txt"));
                    int statusCode = get<0>(status);
                    cout << statusCode << endl;
                }
            }
        }

    #endif

    // 紀錄模型測試時間開始計時
    auto      startTestTime  = std::chrono::high_resolution_clock::now();
    MatrixXd* PhiTestTmp     = get<0>(sqdist_client(testdata, anchor));



    // 生成Test phi
    MatrixXd* PhiTest        = new MatrixXd((-PhiTestTmp->array() * Phi_div).exp().matrix());
    
    // 計算漢明距離
    MatrixXi* tH             = new MatrixXi(((*PhiTest) * (*F_W)).array().sign().matrix().cast<int>());
    *tH                      = (*tH).cwiseMax(0);
    int       hammRadius     = SDH_HammingDist;
    auto      tB_data        = compactbit_client(tH);
    MatrixXi* tB             = get<0>(tB_data);
    auto      hamDist_data   = hammingDist_client(tB, B);
    MatrixXi* hamDist_ptr    = get<0>(hamDist_data);
    MatrixXd* hamDist_SDH    = new MatrixXd(hamDist_ptr->transpose().cast<double>());
    // 設定阈值
    MatrixXd* hamDist_ptr2   = new MatrixXd(MatrixXd::Constant(hamDist_SDH->rows(), hamDist_SDH->cols(), hammRadius + 0.00001) - *hamDist_SDH);
    MatrixXi* Rpred_SDH_ptr1 = new MatrixXi(hamDist_ptr2->array().sign().matrix().cast<int>());
    MatrixXi* Rpred_SDH      = new MatrixXi(Rpred_SDH_ptr1->cwiseMax(0));

    // 紀錄模型測試時間結束計時
    auto      endTestTime    = std::chrono::high_resolution_clock::now();

    // 清除這些變數的動態位址
    delete hamDist_ptr;
    delete hamDist_ptr2;
    delete Rpred_SDH_ptr1;
	
    return std::make_tuple(Rpred_SDH, hamDist_SDH, Bpre, startTrainTime, endTrainTime, startTestTime, endTestTime, start_WaitAggreWeightTime, end_WaitAggreWeightTime);

}
