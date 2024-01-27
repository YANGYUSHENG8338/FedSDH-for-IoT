/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

	Input Parameters
		----------
		Rfact          : array; C - by - Ntest ; 訓練樣本標籤和測試樣本標籤之間的相關性
		Rpred          : array; C - by - Ntest ; 將測試資料對應於標籤結果的 "投票數" 矩陣
		method         : str                   ; 'macro'計算每一筆樣本後再平均 ,  'micro'一次計算整個數據集
	Output Parameters
		----------
		Accuracy       : double ; Accuracy
		Precision      : double ; Precision
		Recall         : double ; Recall
		F1score        : double ; F1score


*/
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <bitset>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include "evaluate_client.h"
using namespace Eigen;
using namespace std;
std::tuple < float , float, float, float > evaluate_client(MatrixXi Rfact, MatrixXi Rpred, string method)
{	
	double Accuracy, Precision, Recall, F1score ;
	double TP = 0, FN = 0, FP = 0, TN = 0;
	if (method == "micro")
	{
		Map<MatrixXi> res_Rfact(Rfact.data(), Rfact.cols() * Rfact.rows(), 1);
		for (int res_Rfact_row = 0; res_Rfact_row < res_Rfact.rows(); res_Rfact_row++)
		{
			if (res_Rfact(res_Rfact_row, 0) == 1)
			{
				res_Rfact(res_Rfact_row, 0) += 1;
			}
		}
		Map<MatrixXi> res_Rpred(Rpred.data(), Rpred.cols() * Rpred.rows(), 1);
		Map<MatrixXi> result(Rpred.data(), Rpred.cols() * Rpred.rows(), 1);
		result = res_Rfact + res_Rpred;
		
		for (int result_row = 0; result_row < result.rows(); result_row++)
		{
			if (result(result_row, 0) == 3)
			{
				TP++;
			}
			if (result(result_row, 0) == 2)
			{
				FN++;
			}
			if (result(result_row, 0) == 1)
			{
				FP++;
			}
			if (result(result_row, 0) == 0)
			{
				TN++;
			}
		}
		
		//Accuracy
		if ((TP + TN + FP + FN) == 0 || (TP + TN) == 0)
		{
			Accuracy = 0;
		}
		else
		{
			Accuracy = (TP + TN) / (TP + TN + FP + FN);
		}
		//Precision
		if ((TP + FP) == 0 || TP == 0)
		{
			Precision = 0;
		}
		else
		{
			Precision = TP / (TP + FP);
		}
		//Recall
		if ((TP + FN) == 0 || TP == 0)
		{
			Recall = 0;
		}
		else
		{
			Recall = TP / (TP + FN);
		}
		//F1score
		if ((Precision + Recall) == 0 || (Precision * Recall) == 0)
		{
			F1score = 0;
		}
		else
		{
			F1score = (2 * Precision * Recall) / (Precision + Recall);
		}

	}
	return std::make_tuple(Accuracy, Precision, Recall, F1score);
}