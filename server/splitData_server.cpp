/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
分割樣本資料集及標籤集
Input Parameters
		----------
		datasetnam           : str                         ; 測試之資料集名稱
		X                    : array  ; D     - by - N     ; 未處理資料
		Eigen_Label          : array  ; D     - by - N     ; 標籤資料
		XtranRatio           : double                      ; 分配給 Xtran 之比例
		XtuneRatio           : double                      ; 分配給 Xtune 之比例
		XtestRatio           : double                      ; 分配給 Xtest 之比例
 Output Parameters
		----------
		Xtran                : array  ; D     - by - Ntran ; 訓練資料矩陣
		Xtune                : array  ; D     - by - Ntune ; 微調資料矩陣
		Xtest                : array  ; D     - by - Ntest ; 測試資料矩陣
		idxtran              : array  ; Ntran - by - 1     ; 訓練資料指標矩陣
		idxtune              : array  ; Ntune - by - 1     ; 測試資料指標矩陣
		idxtest              : array  ; Ntest - by - 1     ; 測試資料指標矩陣
*/
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "uniqLabels_server.h"
#include "splitData_server.h"
using namespace std;
using namespace Eigen;

int convertToEven(int number) {
	if (number < 2) {
		return 2;
	}

	if (number % 2 != 0) {
		return number + 1;
	}

	return number;
}

tuple< MatrixXd, MatrixXd, MatrixXd, MatrixXd, MatrixXd, MatrixXd ,int> splitData_server(string datasetnam, MatrixXd X, VectorXd Eigen_Label, double XtranRatio, double XtuneRatio, double XtestRatio)
{
	double   numtran, numtune, numtest;                                                                        // 將資料集分為 Tran , Tune 和 Test 這三等分，前者變數代表各自的樣本數目
	vector<int> init_idxtran, init_idxtune, init_idxtest, final_idxtest, randomVector;                         // init => 代表原始標籤索引值 ; final => 代表處理後標籤索引值 ;
	numtran                                   = convertToEven(round(X.cols() * XtranRatio));                   // 訓練資料個數
	numtune                                   = convertToEven(round(X.cols() * XtuneRatio));                   // 微調資料個數
	numtest                                   = X.cols() - numtran - numtune;                                  // 測試資料個數
	MatrixXd                 idxtran          = MatrixXd::Zero(1, numtran);                                    // 訓練資料索引值
	MatrixXd                 idxtune          = MatrixXd::Zero(1, numtune);                                    // 微調資料索引值
	MatrixXd                 idxtest;                                                                          // 測試資料索引值
	MatrixXd                 Xtran            = MatrixXd::Zero(X.rows(), numtran);                             // 訓練資料
	MatrixXd                 Xtune            = MatrixXd::Zero(X.rows(), numtune);                             // 微調資料
	MatrixXd                 Xtest;                                                                            // 測試資料

	// 將 Label 小排到大
	auto                     uniqLabels_data  = uniqLabels_server(Eigen_Label);
	vector<double>           Label_uni        = std::get<0>(uniqLabels_data);
	vector<double>::iterator ip               = std::get<1>(uniqLabels_data);
	int                      LabelCount       = Label_uni.size();                                              // 未分裂前完整數據集有幾種標籤
	
	// Displaying the vector after applying std::unique 
	//// 找出 Label 每類個數 
	//for (ip = Label_uni.begin(); ip != Label_uni.end(); ++ip)
	//{
	//	LabelNumber++;
	//}
	
	vector<int> idx;
	for (int m = 0; m < Eigen_Label.rows(); m++)
	{
		idx.push_back(m);
	}
	// 亂數打亂每個類別的索引值
	randomVector.assign(idx.begin(), idx.end());
	random_device seed_gen;
	mt19937 engine(seed_gen());
	shuffle(randomVector.begin(), randomVector.end(), engine);
	init_idxtran.assign(randomVector.begin(), randomVector.begin() + numtran);
	init_idxtune.assign(randomVector.begin() + numtran, randomVector.begin() + numtran + numtune);
	init_idxtest.assign(randomVector.begin() + numtran + numtune, randomVector.begin() + numtran + numtune + numtest);

	if (datasetnam.compare("YALE") == 0)
	{
		idxtest = MatrixXd::Zero(1, numtest * 2);
		final_idxtest = init_idxtest;
		final_idxtest.insert(final_idxtest.end(), init_idxtest.begin(), init_idxtest.end());
	}
	else
	{
		idxtest = MatrixXd::Zero(1, numtest);
		final_idxtest.insert(final_idxtest.end(), init_idxtest.begin(), init_idxtest.end());
	}
	// YALE 將 test 多複製一份(15個樣本)   => 原因是依照比例分配，test 最終只能分配到 15 筆樣本數目，如果將這 15 筆測試樣本分給兩個裝置會造成分配不均的問題，因此將此測試樣本再多複製一份，不僅能夠實現樣本數的均勻分配，還能提升該裝置的評估指標。 






	// 將 Vector 轉換為 Matrix
	for (int i = 0; i < idxtran.rows(); i++)
	{
		for (int j = 0; j < idxtran.cols(); j++)
		{
			idxtran(i, j) = init_idxtran[j];
		}
	}

	for (int i = 0; i < idxtune.rows(); i++)
	{
		for (int j = 0; j < idxtune.cols(); j++)
		{
			idxtune(i, j) = init_idxtune[j];
		}
	}

	for (int i = 0; i < idxtest.rows(); i++)
	{
		for (int j = 0; j < idxtest.cols(); j++)
		{
			idxtest(i, j) = final_idxtest[j];
		}
	}

	//YALE使用
	if (datasetnam.compare("YALE") == 0)
	{
		Xtest = MatrixXd::Zero(X.rows(), numtest * 2);
	}
	else
	{
		Xtest = MatrixXd::Zero(X.rows(), numtest);
	}

	// 將 Label 索引值對應的資料轉換為 Xtran , Xtune 和 Xtest
	for (int n = 0; n < Xtran.cols(); n++)
	{
		for (int m = 0; m < X.rows(); m++)
		{
			Xtran(m, n) = X(m, init_idxtran[n]);
		}
	}
	for (int n = 0; n < Xtune.cols(); n++)
	{
		for (int m = 0; m < X.rows(); m++)
		{
			Xtune(m, n) = X(m, init_idxtune[n]);
		}
	}
	for (int n = 0; n < Xtest.cols(); n++)
	{
		for (int m = 0; m < X.rows(); m++)
		{
			Xtest(m, n) = X(m, final_idxtest[n]);
		}
	}

	return make_tuple(Xtran, Xtune, Xtest, idxtran, idxtune, idxtest, LabelCount); //LabelCount==未分裂前完整數據集有幾種標籤
}
