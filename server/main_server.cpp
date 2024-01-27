/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

    Input Parameters
        ----------
    x_lin           : array ; D - by - N ;
    t               : array ; N - by - 1 ;(LabelEncoder)
    Output Parameters
        ----------
    RASP
        rasp_Xtran           : array  ; D                       - by - (Ntran * raspDataRatio) ; 發送至 Rasp 訓練資料矩陣
        rasp_Xtune           : array  ; D                       - by - (Ntune * raspDataRatio) ; 發送至 Rasp 微調資料矩陣
        rasp_Xtest           : array  ; D                       - by - (Ntest * raspDataRatio) ; 發送至 Rasp 測試資料矩陣
        rasp_ttran           : array  ; (Ntran * raspDataRatio) - by - 1                       ; 發送至 Rasp 訓練標籤矩陣(LabelEncoder)
        rasp_ttune           : array  ; (Ntune * raspDataRatio) - by - 1                       ; 發送至 Rasp 微調標籤矩陣(LabelEncoder)
        rasp_ttest           : array  ; (Ntest * raspDataRatio) - by - 1                       ; 發送至 Rasp 測試標籤矩陣(LabelEncoder)
    NANO
        nano_Xtran           : array  ; D                       - by - (Ntran * nanoDataRatio) ; 發送至 Nano 訓練資料矩陣
        nano_Xtune           : array  ; D                       - by - (Ntune * nanoDataRatio) ; 發送至 Nano 微調資料矩陣
        nano_Xtest           : array  ; D                       - by - (Ntest * nanoDataRatio) ; 發送至 Nano 測試資料矩陣
        nano_ttran           : array  ; (Ntran * nanoDataRatio) - by - 1                       ; 發送至 Nano 訓練標籤矩陣(LabelEncoder)
        nano_ttune           : array  ; (Ntune * nanoDataRatio) - by - 1                       ; 發送至 Nano 微調標籤矩陣(LabelEncoder)
        nano_ttest           : array  ; (Ntest * nanoDataRatio) - by - 1                       ; 發送至 Nano 測試標籤矩陣(LabelEncoder)


補充說明-------------------------------------------------------------------------------------------------------------------------
***資料集***
    以下四個資料集是透過使用實驗室格式資料集進行製作的。主要目的是將 MAT 檔格式轉換成 TXT 檔，因為原始 MAT 檔僅包含數字資料，所以 TXT 檔的編碼方式僅使用 ASCII 進行編碼。同時，還將 MAT 檔中的標籤資料從 OneHotEncoder 形式轉換為 LabelEncoder 形式。
    載入實驗室 mat 資料集以後，執行以下指令
    save(轉換為 txt 檔的名稱, 從 MAT 檔中獲取的矩陣, 以ASCII編碼寫入 txt)
    save("x_lin.txt"        ,"x_lin"               ,"-ASCII")
    save("t.txt"            ,"t"                   ,"-ASCII")            // t 需轉成 LabelEncode 再存 txt 檔
        MAT   x_lin   : array ; D - by - N ; 未處理資料                =>   TXT   x_lin   : array ; D - by - N ;
        MAT   t       : array ; N - by - C ; 標籤資料(OneHotEncode)   =>   TXT   t       : array ; N - by - 1 ;(LabelEncode)
*/

#pragma warning( disable : 4996 )
#include <stdio.h>
#include <iostream>
#include <thread>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cstdlib>
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
#include "splitData_server.h"
#include "LoadData_server.h"
#include "FileExists_server.h"
#include "int2txt_server.h"
#include "double2txt_server.h"
#include "EmptyWeightPool_server.h"
#include "SendOrReturnWeight_server.h"
#include "GetCurrentTimeStamp_server.h"
#include "compareLableClass_server.h"
#include "AggregatedWWeight_server.h"
#include "main_server.h"
#include "../initpath.h"

// @@@寫一個readme 從安裝到執行的步驟
using namespace Eigen;
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // nanoseconds, system_clock, seconds
namespace fs = std::filesystem;


struct param_struct {
    int    nExps;                                    // 執行幾回完整的模型更新
    int    J;                                        // 錨點
    int    L;                                        // 編碼長度
    int    AggreIter;                                // 最大伺服端聚合輪次
    double XtranRatio;                               // 分配給Xtran之比例
    double XtuneRatio;                               // 分配給Xtran之比例
    double XtestRatio;                               // 分配給Xtran之比例
}param;




extern int main(int argc, char** argv)
{
    // 參數設定
    string datasetnam         = "ORL";                       // 輸入需測試資料集("YALE", "JAFFE", "ORL", "SMALL_ARFACE")
    vector<int> bitlen        = { 16, 32 , 64, 96, 128 };    // (只允許一次一種碼長 因有bug)程式中須測試的碼長，主要碼長設定在param.L;   16, 32 , 64, 96, 128
    int nChannels             = 1;                           // if RGB, nChannels = 3
    param.nExps               = 1;                           // 執行幾回完整的模型更新
    param.J                   = 0;                           // 錨點; 0 means all training data
    param.L                   = 64;                          // (無效 因會被bitlen設定) 編碼長度(先宣告，64為預設值)
    param.AggreIter           = 5;                           // 最大伺服端聚合輪次
    param.XtranRatio          = 0.6;                         // 分配給 Xtran 之比例
    param.XtuneRatio          = 0.1;                         // 分配給 Xtune 之比例
    param.XtestRatio          = 1 - param.XtranRatio - param.XtuneRatio;                         // 分配給 Xtest 之比例
    double raspDataRatio      = 0.5;                         // 分配資料給 RASP 之比例
    double nanoDataRatio      = 1 - raspDataRatio;           // 分配資料給 NANO 之比例
    string weightPoolPath     = INIT_SERVER_weightPoolPath;  // 查詢此路徑下資料夾的聚合權重是否清空
    string CtrlSendFile       = INIT_SERVER_ControlFile;     // 控制訊號存放位置
    string CTRL_MSG_1         = INIT_SERVER_CTRL_MSG_1;      // SERVER 端 傳送 TRAIN、TUNE 和 TEST 到 NODE   端 ###也要放到INITPATH
    string CTRL_MSG_2         = INIT_SERVER_CTRL_MSG_2;      // NODE   端 傳送 局部更新權重        到 SERVER 端
    string CTRL_MSG_3         = INIT_SERVER_CTRL_MSG_3;      // SERVER 端 傳送 全局聚合權重        到 NODE   端
    string CTRL_MSG_4         = INIT_SERVER_CTRL_MSG_4;      // SERVER 端 傳送 ServerEnd           到 NODE   端
    string CTRL_MSG_5         = INIT_SERVER_CTRL_MSG_5;      // NODE   端 傳送 ClientEnd           到 SERVER 端
    int timeout_wait4WP_limit = 168;                         // 當掃描此資料集是否有指定權重的上限次數
    // 變數宣告
    string infilepath_x_lin_data;                            // 樣本資料路徑
    string infilepath_t_data;                                // 標籤資料路徑

    // 將 shellCode 需用到的路徑及搜尋的字串寫入 ShellPath.txt，讓 shellCode 讀取
    string shell2SERVERPath   = INIT_SERVER_Control2SERVERPath;
    string shell2RASPPath     = INIT_SERVER_Control2RASPPath;
    string shell2NANOPath     = INIT_SERVER_Control2NANOPath;
    std::ofstream outfile("shellPath.txt");  // 創建輸出文件流對象

    if (outfile.is_open()) {  // 確保文件成功打開
        outfile << shell2SERVERPath             << std::endl;
        outfile << shell2RASPPath               << std::endl;
        outfile << shell2NANOPath               << std::endl;
        outfile << CTRL_MSG_1                   << std::endl;
        outfile << CTRL_MSG_2                   << std::endl;
        outfile << CTRL_MSG_3                   << std::endl;
        outfile << CtrlSendFile                 << std::endl;
        outfile.close();  // 關閉文件
        cout    << "字串已成功寫入 shellPath。" << std::endl;
    }
    else {
        cout    << "無法打開 shellPath。"       << std::endl;
    }


    // 取用資料集(請看成是第 30 行)
    // YALE
    if (datasetnam.compare("YALE") == 0)
    {
        infilepath_x_lin_data = "../Dataset/yale64x64_data_x_lin.txt";
        infilepath_t_data     = "../Dataset/yale64x64_data_t.txt";
    }
    // JAFFE
    else if (datasetnam.compare("JAFFE") == 0)
    {
        infilepath_x_lin_data = "..//Dataset/jaffe64x64_data_x_lin.txt";
        infilepath_t_data     = "../Dataset/jaffe64x64_data_t.txt";
    }
    // ORL
    else if (datasetnam.compare("ORL") == 0)
    {
        infilepath_x_lin_data = "../Dataset/orl64x64_data_x_lin.txt";
        infilepath_t_data     = "../Dataset/orl64x64_data_t.txt";
    }
    // SMALL_ARFACE
    else if (datasetnam.compare("SMALL_ARFACE") == 0)
    {
        infilepath_x_lin_data = "../Dataset/SMALL_2340_ARface64x64_data_x_lin.txt";
        infilepath_t_data     = "../Dataset/SMALL_2340_ARface64x64_data_t.txt";
    }

    // 載入資料集和標籤
    auto x_lin_data = LoadData_server(infilepath_x_lin_data);
    auto t_data     = LoadData_server(infilepath_t_data);
    MatrixXd x_lin  = get<0>(x_lin_data);
    MatrixXd t      = get<0>(t_data);
    MatrixXd Xfull  = x_lin / 255;
    VectorXd Label  = t.transpose().row(0);


    

    for (int testBitIdx = 0; testBitIdx < bitlen.size(); testBitIdx++)
    {
        param.L = bitlen[testBitIdx];                          // 編碼長度
        EmptyWeightPool_server(weightPoolPath);                // 查詢此資料夾是否清空
        if (testBitIdx > 0)
        {
            //string ServerEnd = INIT_SERVER_ControlServerEnd;
            //string ClientEnd = INIT_SERVER_ControlClientEnd;
            //std::string CtrlNANOFile = folderPath + INIT_SERVER_ControlSERVERNANOFile;
            //std::string CtrlRASPFile = folderPath + INIT_SERVER_ControlSERVERRASPFile;
            //std::ofstream Startfile1(CtrlNANOFile);  // 調解SERVER 和 CLIENT 清空資料夾的時機
            //std::ofstream Startfile2(CtrlRASPFile);  // 調解SERVER 和 CLIENT 清空資料夾的時機
            //if (Startfile1.is_open()) {  // 確保文件成功打開
            //    Startfile1 << ServerEnd << std::endl;
            //    Startfile1.close();  // 關閉文件
            //    cout << "字串已成功寫入 ControlSERVERNANOFile。" << std::endl;
            //}
            //else {
            //    cout << "無法打開 ControlSERVERNANOFile。" << std::endl;
            //}
            //if (Startfile2.is_open()) {  // 確保文件成功打開
            //    Startfile2 << ServerEnd << std::endl;
            //    Startfile2.close();  // 關閉文件
            //    cout << "字串已成功寫入 ControlSERVERRASPFile。" << std::endl;
            //}
            //else {
            //    cout << "無法打開 ControlSERVERRASPFile。" << std::endl;
            //}
            //SendOrReturnWeight_server(CTRL_MSG_4); // 將控制指令(發送檔案至NODE)傳送到特定的控制檔案中
            //cout << "發送 ControlSERVERCLIENTFile 到 Client。" << std::endl;
            //std::ifstream Startfile3(CtrlNANOFile);
            //std::ifstream Startfile4(CtrlRASPFile);
            //std::string line;//逐行讀取文件內容並檢查是否包含特定字串

            ////while (std::getline(Startfile3, line)) {
            ////    if (line.find(ClientEnd) != std::string::npos) {
            ////        std::cout << "The file contains the search string." << std::endl;
            ////    }
            ////    std::this_thread::sleep_for(std::chrono::seconds(1));
            ////}
            //bool foundInFile1 = false;
            //bool foundInFile2 = false;

            //while (!foundInFile1 || !foundInFile2) {
            //    if (!foundInFile1 && std::getline(Startfile3, line)) {
            //        if (line.find(ClientEnd) != std::string::npos) {
            //            std::cout << "The search string was found in file 1." << std::endl;
            //            foundInFile1 = true;
            //        }
            //    }

            //    if (!foundInFile2 && std::getline(Startfile4, line)) {
            //        if (line.find(ClientEnd) != std::string::npos) {
            //            std::cout << "The search string was found in file 2." << std::endl;
            //            foundInFile2 = true;
            //        }
            //    }

            //    if (!foundInFile1 || !foundInFile2) {
            //        SendOrReturnWeight_server(CTRL_MSG_5); // 將控制指令(發送檔案至NODE)傳送到特定的控制檔案中
            //        std::this_thread::sleep_for(std::chrono::seconds(1));
            //    }
            //}
        }
        for (int expid = 0; expid < param.nExps; expid++)
        {
            // 將原始資料集和標籤按比例分成訓練、微調和測試資料集
            auto split_data      = splitData_server(datasetnam, Xfull, Label, param.XtranRatio, param.XtuneRatio, param.XtestRatio);
            MatrixXd Xtran       = std::get<0>(split_data);
            MatrixXd Xtune       = std::get<1>(split_data);
            MatrixXd Xtest       = std::get<2>(split_data);
            MatrixXd idxtran     = std::get<3>(split_data);
            MatrixXd idxtune     = std::get<4>(split_data);
            MatrixXd idxtest     = std::get<5>(split_data);
            int      LabelCount  = std::get<6>(split_data); //LabelCount==未分裂前完整數據集有幾種標籤
            MatrixXi ttran       = MatrixXi::Zero(Xtran.cols(), 1);
            MatrixXi ttune       = MatrixXi::Zero(Xtune.cols(), 1);
            MatrixXi ttest       = MatrixXi::Zero(Xtest.cols(), 1);

            for (int m = 0; m < ttran.rows(); m++)
            {
                int a = idxtran(0, m);
                ttran(m, 0) = int(Label(a));
            }
            for (int n = 0; n < ttune.rows(); n++)
            {
                int a = idxtune(0, n);
                ttune(n, 0) = int(Label(a));
            }
            unsigned int addcol_test = 0;
            for (int o = 0; o < ttest.rows(); o++)
            {
                int a = idxtest(0, o);
                ttest(o, 0) = int(Label(a));
            }


            
            printf("$$$$$$$$$$\nServer_assign_data\n$$$$$$$$$$\n");
            MatrixXd rasp_Xtran = Xtran.leftCols(round(Xtran.cols() * raspDataRatio));
            MatrixXd rasp_Xtune = Xtune.leftCols(round(Xtune.cols() * raspDataRatio));
            MatrixXd rasp_Xtest = Xtest.leftCols(round(Xtest.cols() * raspDataRatio));
            MatrixXd nano_Xtran = Xtran.rightCols(round(Xtran.cols() * nanoDataRatio));
            MatrixXd nano_Xtune = Xtune.rightCols(round(Xtune.cols() * nanoDataRatio));
            MatrixXd nano_Xtest = Xtest.rightCols(round(Xtest.cols() * nanoDataRatio));
            // 分發後 Data 儲存到特定路徑下
            string str_EXP = to_string(expid);
            string RASP_INIT_X_TRAN = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_XtranMtxTxt;
            string RASP_INIT_X_TUNE = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_XtuneMtxTxt;
            string RASP_INIT_X_TEST = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_XtestMtxTxt;
            string NANO_INIT_X_TRAN = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_XtranMtxTxt;
            string NANO_INIT_X_TUNE = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_XtuneMtxTxt;
            string NANO_INIT_X_TEST = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_XtestMtxTxt;
            // 分發後 Data 儲存到特定路徑下
            double2txt_server(rasp_Xtran, RASP_INIT_X_TRAN);
            double2txt_server(rasp_Xtune, RASP_INIT_X_TUNE);
            double2txt_server(rasp_Xtest, RASP_INIT_X_TEST);
            double2txt_server(nano_Xtran, NANO_INIT_X_TRAN);
            double2txt_server(nano_Xtune, NANO_INIT_X_TUNE);
            double2txt_server(nano_Xtest, NANO_INIT_X_TEST);

            // 將 Label 按比例分發給 RASP & NANO
            MatrixXi rasp_ttran           = ttran.topRows   (round(Xtran.cols() * raspDataRatio));
            MatrixXi rasp_ttune           = ttune.topRows   (round(Xtune.cols() * raspDataRatio));
            MatrixXi rasp_ttest           = ttest.topRows   (round(Xtest.cols() * raspDataRatio));
            MatrixXi nano_ttran           = ttran.bottomRows(round(Xtran.cols() * nanoDataRatio));
            MatrixXi nano_ttune           = ttune.bottomRows(round(Xtune.cols() * nanoDataRatio));
            MatrixXi nano_ttest           = ttest.bottomRows(round(Xtest.cols() * nanoDataRatio));
            MatrixXi LABELCOUNT           = MatrixXi::Zero(1, 1);
            LABELCOUNT(0, 0)              = LabelCount;//LabelCount==未分裂前完整數據集有幾種標籤
            string RASP_INIT_L_TRAN       = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_LtranMtxTxt;
            string RASP_INIT_L_TUNE       = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_LtuneMtxTxt;
            string RASP_INIT_L_TEST       = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_LtestMtxTxt;
            string NANO_INIT_L_TRAN       = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_LtranMtxTxt;
            string NANO_INIT_L_TUNE       = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_LtuneMtxTxt;
            string NANO_INIT_L_TEST       = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_LtestMtxTxt;
            string RASP_INIT_L_LABELCOUNT = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + INIT_SERVER_LabelCountMtxTxt;
            string NANO_INIT_L_LABELCOUNT = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + INIT_SERVER_LabelCountMtxTxt;

            // 分發後 Label 儲存到特定路徑下
            int2txt_server(rasp_ttran, RASP_INIT_L_TRAN);
            int2txt_server(rasp_ttune, RASP_INIT_L_TUNE);
            int2txt_server(rasp_ttest, RASP_INIT_L_TEST);
            int2txt_server(nano_ttran, NANO_INIT_L_TRAN);
            int2txt_server(nano_ttune, NANO_INIT_L_TUNE);
            int2txt_server(nano_ttest, NANO_INIT_L_TEST);
            int2txt_server(LABELCOUNT, RASP_INIT_L_LABELCOUNT);
            int2txt_server(LABELCOUNT, NANO_INIT_L_LABELCOUNT);

            SendOrReturnWeight_server(CTRL_MSG_1); // 將控制指令(發送檔案至NODE)傳送到特定的控制檔案中



            for (int aggreItr = 0; aggreItr < param.AggreIter; aggreItr++)            // param.AggreIter => 聚合權重次數
            {
                if (aggreItr > (param.AggreIter - 2))            // 例如:聚合權重次數(param.AggreIter)為 5 次，扣掉最後一次為使用測試資料集不聚合，及第 4 次該局部權重已獲得該次的評估指標，所以就不聚合，因此聚合次數為3次
                {
                    cout << "第 4 次跳出更新\t" << endl;
                    break;
                }
                string str_ITR = to_string(aggreItr);
                // 設定這些路徑原因是為了取得 client 端傳回的局部更新權重的路徑
                string RASP_WPath = weightPoolPath + INIT_SERVER_RASP2SERVER + str_EXP + "_" + str_ITR + INIT_SERVER_WWeightMtxTxt; //RASP_WPath_RASP2SERVER_TOKEN RASP_WPath__WWeight_matrix_TOKEN
                string RASP_PPath = weightPoolPath + INIT_SERVER_RASP2SERVER + str_EXP + "_" + str_ITR + INIT_SERVER_PWeightMtxTxt;
                string NANO_WPath = weightPoolPath + INIT_SERVER_NANO2SERVER + str_EXP + "_" + str_ITR + INIT_SERVER_WWeightMtxTxt;
                string NANO_PPath = weightPoolPath + INIT_SERVER_NANO2SERVER + str_EXP + "_" + str_ITR + INIT_SERVER_PWeightMtxTxt;
                //
                string RASP_labelListPath = weightPoolPath + INIT_SERVER_RASP2SERVER + str_EXP + INIT_SERVER_LabelListMtxTxt;
                string NANO_labelListPath = weightPoolPath + INIT_SERVER_NANO2SERVER + str_EXP + INIT_SERVER_LabelListMtxTxt;
                int timeout_wait4WP = 0;
                // 下面的 while 迴圈是等待 NODE 端傳送局部更新權重到 SERVER 端
                while (((get<0>(FileExists_server(RASP_WPath))) && (get<0>(FileExists_server(RASP_PPath))) && (get<0>(FileExists_server(NANO_WPath))) && (get<0>(FileExists_server(NANO_PPath))) && (get<0>(FileExists_server(RASP_labelListPath))) && (get<0>(FileExists_server(NANO_labelListPath)))) == false)
                {

                    cout << "\t 第" + str_EXP + "_" + str_ITR + "次更新\t";
                    printf("Wait for node sending files...\n");
                    // 將控制訊號調成第二種模式(NODE 端傳送局部更新權重到 SERVER 端)
                    SendOrReturnWeight_server(CTRL_MSG_2);
                    timeout_wait4WP++;
                    if (timeout_wait4WP > timeout_wait4WP_limit)//當這個迴圈偵測這四檔案沒在 weightPoolPath 資料夾中，並偵測次數到上限值，就跳出迴圈
                    {
                        timeout_wait4WP = 0;
                        break;
                    }
                    cout << "第" << timeout_wait4WP << "次等待" << endl;
                    // 獲取當時時間點
                    auto GetCurrentTimeStamp_data = GetCurrentTimeStamp_server(3);
                    std::cout << get<0>(GetCurrentTimeStamp_data) << endl;
                    // 休眠1000毫秒(第一次)
                    // 每一秒鐘偵測這四檔案有無在 weightPoolPath 資料夾中
                    auto now_1 = std::chrono::steady_clock::now();
                    auto sleep_until_1 = now_1 + std::chrono::milliseconds(1000);
                    std::this_thread::sleep_until(sleep_until_1);
                }
                // 將 RASP 傳回的局部更新權重和 NANO 傳回的局部更新權重進行聚合
                printf("$$$$$$$$$$\nServer_Aggregation_Matrix\n$$$$$$$$$$\n");
                MatrixXd RENEW_RASP_WWeight       = std::get<0>(LoadData_server(RASP_WPath)); // client 端傳回的局部更新權重檔會放在RASP_WPath的路徑中，server 從此路徑載入該檔案
                MatrixXd RENEW_RASP_PWeight       = std::get<0>(LoadData_server(RASP_PPath));
                MatrixXd RENEW_NANO_WWeight       = std::get<0>(LoadData_server(NANO_WPath));
                MatrixXd RENEW_NANO_PWeight       = std::get<0>(LoadData_server(NANO_PPath));

                // 取得該裝置得到標籤有哪幾類
                MatrixXd RENEW_RASP_labelList     = std::get<0>(LoadData_server(RASP_labelListPath));
                MatrixXd RENEW_NANO_labelList     = std::get<0>(LoadData_server(NANO_labelListPath));
                // 合併 PWeight 矩陣
                MatrixXd AGGREGATED_PWeight = RENEW_RASP_PWeight * raspDataRatio + RENEW_NANO_PWeight * nanoDataRatio;
                // 合併 WWeight 矩陣
                auto AggregatedWWeight_server_data = AggregatedWWeight_server(RENEW_RASP_labelList, RENEW_NANO_labelList, RENEW_RASP_PWeight, RENEW_NANO_WWeight, LabelCount, param.L, raspDataRatio, nanoDataRatio);
                MatrixXd AGGREGATED_RASP_WWeight = get<0>(AggregatedWWeight_server_data);
                MatrixXd AGGREGATED_NANO_WWeight = get<1>(AggregatedWWeight_server_data);



                
                printf("$$$$$$$$$$\nServer_Assign_NEW_Matrix\n$$$$$$$$$$\n");
                // 將 SERVER 端聚合完的 W 和 P 權重傳回 節點端(RASP & NANO)
                string After_Aggre_Itr = to_string(aggreItr + 1);     // 聚合完後將聚合次數加 1
                // 製作聚合完 W 和 P 的全局權重路徑
                string SERVER2RASP_AGGREGATED_PWeight = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + "_" + After_Aggre_Itr + INIT_SERVER_PWeightMtxTxt;
                string SERVER2RASP_AGGREGATED_WWeight = weightPoolPath + INIT_SERVER_SERVER2RASP + str_EXP + "_" + After_Aggre_Itr + INIT_SERVER_WWeightMtxTxt;
                string SERVER2NANO_AGGREGATED_PWeight = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + "_" + After_Aggre_Itr + INIT_SERVER_PWeightMtxTxt;
                string SERVER2NANO_AGGREGATED_WWeight = weightPoolPath + INIT_SERVER_SERVER2NANO + str_EXP + "_" + After_Aggre_Itr + INIT_SERVER_WWeightMtxTxt;
                // 聚合完 W 和 P 的全局權重存到 weightPoolPath 資料夾中
                double2txt_server(AGGREGATED_PWeight      , SERVER2RASP_AGGREGATED_PWeight);
                double2txt_server(AGGREGATED_RASP_WWeight , SERVER2RASP_AGGREGATED_WWeight);
                double2txt_server(AGGREGATED_PWeight      , SERVER2NANO_AGGREGATED_PWeight);
                double2txt_server(AGGREGATED_NANO_WWeight , SERVER2NANO_AGGREGATED_WWeight);
                // 休眠 1000 毫秒(第四次)
                auto now_4 = std::chrono::steady_clock::now();
                auto sleep_until_4 = now_4 + std::chrono::milliseconds(3000);
                std::this_thread::sleep_until(sleep_until_4);
                //將控制訊號調成第三種模式
                SendOrReturnWeight_server(CTRL_MSG_3);
                // 獲取當時時間點
                auto GetCurrentTimeStamp_data = GetCurrentTimeStamp_server(3);
                std::cout << get<0>(GetCurrentTimeStamp_data) << endl;
                // 休眠 1000 毫秒(第二次)
                auto now_2 = std::chrono::steady_clock::now();
                auto sleep_until_2 = now_2 + std::chrono::milliseconds(3000);
                std::this_thread::sleep_until(sleep_until_2);
            }

        }

        // 獲取當時時間點
        auto GetCurrentTimeStamp_data = GetCurrentTimeStamp_server(3);
        std::cout << get<0>(GetCurrentTimeStamp_data) << endl;
        // 休眠 3000 毫秒(第三次) => 等待客戶端不再占用檔案 
        auto now_3 = std::chrono::steady_clock::now();
        auto sleep_until_3 = now_3 + std::chrono::milliseconds(3000);
        std::this_thread::sleep_until(sleep_until_3);
        if ((datasetnam.compare("SMALL_ARFACE") == 0))                                      // 等待 Server 端刪除完共享資料夾的東西不然會出錯
        {
            auto waitTime = steady_clock::now();
            auto sleepTime = waitTime + milliseconds(5000);
            std::this_thread::sleep_until(sleepTime);
        }

    }
    return 0;
}

