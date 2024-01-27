/*取得mAP
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式
	Input Parameters 
		----------
		Ypred      : array; 大小 N - by - L; the predicted outputs of the classifier, the output of the ith instance for the jth class is stored in Ypred(i,j)   
		Ytrue      : array; 大小 N - by - L; the actual labels of the test instances, if the ith instance belong to the jth class, Ytrue(i,j)=1, otherwise Ytrue(i,j)=0  
		L 為標籤種類數
		N 為樣本總數  
	
	Output Parameters 
		----------  
		Average_Precision : double; mean Average Precision



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
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "multilabelmaPrecision_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;

void eigen_sort_cols_by_head(Eigen::MatrixXi& A_nx3)
{
	std::vector<Eigen::VectorXi> vec;
	for (int64_t i = 0; i < A_nx3.cols(); ++i)
		vec.push_back(A_nx3.col(i));

	std::sort(vec.begin(), vec.end(), [](Eigen::VectorXi const& t1, Eigen::VectorXi const& t2) { return t1(0) < t2(0); });

	for (int64_t i = 0; i < A_nx3.cols(); ++i)
		A_nx3.col(i) = vec[i];
}

void RemoveRow(Eigen::MatrixXi& matrix, unsigned int rowToRemove) {
	unsigned int numRows = matrix.rows() - 1;
	unsigned int numCols = matrix.cols();

	if (rowToRemove < numRows) {
		matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) = matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);
	}

	matrix.conservativeResize(numRows, numCols);
}

std::tuple <double> multilabelmaPrecision_client(MatrixXi Ytrue, MatrixXi Ypred)
{
    int      num_class     = Ypred.cols();
	MatrixXi sum_result    = Ytrue.rowwise().sum();
	MatrixXi temp_outputs  = Ypred;
	MatrixXi temp_test     = Ytrue;
	//cout << "temp_outputs = " << temp_outputs << endl;
	//cout << "temp_test = " << temp_test << endl;
	//cout << "sum_result = " << sum_result << endl;
	//刪除全0的結果
	// find non-zero columns:
	Matrix<bool, Dynamic, 1> non_zeros = sum_result.cast<bool>().rowwise().any();
	// allocate result matrix:
	MatrixXi temp_outputs_res = MatrixXi::Zero(non_zeros.count(), temp_outputs.cols());
	MatrixXi temp_test_res = MatrixXi::Zero(non_zeros.count(), temp_outputs.cols());
	Index j = 0;
	for (Eigen::Index i = 0; i < temp_outputs.rows(); ++i)
	{
		if (non_zeros(i))
		{
			temp_outputs_res.row(j) = temp_outputs.row(i);
			temp_test_res.row(j) = temp_test.row(i);
			j++;
		}
			
	}
	//cout << "temp_outputs_res = " << temp_outputs_res << endl;
	//cout << "temp_test_res = " << temp_test_res << endl;
	//刪除全1的結果
	for (int i = 0; i < sum_result.rows(); i++)
	{
		if (sum_result(i,0) == num_class)
		{
			RemoveRow(temp_outputs_res, i);
			RemoveRow(temp_test_res, i);
		}
	}
	//cout << "temp_outputs_res = " << temp_outputs_res << endl;
	//cout << "temp_test_res = " << temp_test_res << endl;
	// 2. 
	num_class = temp_outputs_res.cols();
	int num_instance = temp_outputs_res.rows();
	MatrixXi label_x     = MatrixXi::Zero(temp_test_res.cols(), 1);
	MatrixXi label       = MatrixXi::Zero(temp_test_res.cols(), 1);
	MatrixXi not_label_x = MatrixXi::Zero(temp_test_res.rows() * temp_test_res.cols() - temp_test_res.cols(), 1);
	MatrixXi not_label   = MatrixXi::Zero(temp_test_res.rows() * temp_test_res.cols() - temp_test_res.cols(), 1);
	int a = 0, b = 0, c = 0, d = 0;
	//cout << "temp_test = " << temp_test << endl;
	//cout << "temp_test_res = " << temp_test_res << endl;
	for (int i = 0; i < temp_test_res.cols(); i++)
	{
		for (int j = 0; j < temp_test_res.rows(); j++)
		{
			if (temp_test_res(j, i) == 1)
			{
				label_x(a, 0) = j;
				label(a, 0) = a;
				//cout << "label_x = " << label_x.transpose() << endl;
				//cout << "label = " << label.transpose() << endl;
				a++;

			}
			else if (temp_test_res(j, i) != 1)
			{
				
				not_label_x(b, 0) = j;
				not_label(b, 0) = b;
				//cout << "not_label_x = " << not_label_x.transpose() << endl;
				//cout << "not_label = " << not_label.transpose() << endl;
				b++;

			}
			
		}
	}
	MatrixXi label_size = temp_test_res.rowwise().sum();
	//cout << "label_size = " << label_size.transpose() << endl;
	//MatrixXi label_size1 = temp_test.rowwise().sum();
	//cout << "label_size1 = " << label_size1.transpose() << endl;
	//3
	double aveprec = 0;
	for (int i = 0; i < num_instance; i++)
	{
		MatrixXi tempvalue = temp_outputs_res.row(i);
		//cout << "tempvalue = " << tempvalue << endl;

		eigen_sort_cols_by_head(tempvalue);
		//cout << "tempvalue = " << tempvalue << endl;
		vector<int> index, label_t;
		index.push_back(-1);

		for (int k = 0; k < tempvalue.cols(); k++)
		{
			for (int j = 0; j < temp_outputs_res.cols(); j++)
			{
				int m = count(index.begin(), index.end(), j);
				//cout << "count = " << count(index.begin(), index.end(), j) << endl;
				//cout << "temp_outputs_res = " << temp_outputs_res(i, j) << endl;
				//cout << "tempvalue = " << tempvalue << endl;
				if (temp_outputs_res(i, j) == tempvalue(0, k) && !(count(index.begin(), index.end(), j)))
				{
					index.push_back(j);
                }
			}
			//for (const auto  number: index) {
			//	//cout << "index1 = " << number << endl;
			//}
			
			auto iter = remove(index.begin(), index.end(), -1);

			//for (const auto number : index) {
			//	//cout << "index2 = " << number << endl;
			//}
			index.erase(iter, index.end());
			//for (const auto number : index) {
			//	//cout << "index3 = " << number << endl;
			//}

		}
		int loc;
		MatrixXi indicator = MatrixXi::Zero(1, 2);
		MatrixXi append_vec(1, 2);
		//cout << "label_size = " << label_size.transpose() << endl;
		for (int m = 0; m < label_size(i, 0); m++)
		{
			loc = -1;
			for (int loc_temp = 0; loc_temp < index.size(); loc_temp++)
			{
				vector <int>().swap(label_t);
				for (int n = 0; n < label_x.rows(); n++)
				{
					//cout << "label_x = " << label_x.transpose() << endl;
					//cout << "label = " << label.transpose() << endl;

					if (label_x(n, 0) == i)
					{
						label_t.push_back(label(n, 0));
					}
				}
				if (label_t[m] == index[loc_temp])
				{
					loc = loc_temp; break;
				}
			}
			append_vec << loc, 1;
			//cout << "append_vec = " << append_vec << endl;
			indicator.conservativeResize(indicator.rows() + 1, indicator.cols());
			indicator.row(indicator.rows() - 1) = append_vec;
			//cout << "indicator = " << indicator << endl;

		}
		RemoveRow(indicator, 0);
		double summary = 0;
		for (int m = 0; m < label_size(i, 0); m++)
		{
			loc = num_class;
			for (int loc_temp = 0; loc_temp < index.size(); loc_temp++)
			{
				vector <int>().swap(label_t);
				for (int n = 0; n < label_x.rows(); n++)
				{
					if (label_x(n, 0) == i)
					{
						label_t.push_back(label(n, 0));
					}
				}
				if (label_t[m] == index[loc_temp])
				{
					loc = loc_temp; break;
				}
			}
			int sencond_up = 0;
			for (int loc_i = loc; loc_i < num_class; loc_i++)
			{
				for (int p = 0; p < indicator.rows(); p++)
				{
					if (indicator(p, 0) == loc_i)
					{
                        sencond_up = sencond_up + 1;
					}	
				}
			}
			summary = summary + (sencond_up * 1.0 / (num_class - 1 - (loc) + 1));
		}
		aveprec = aveprec + summary * 1.0 / label_size(i, 0);
	}
	double Average_Precision;
	Average_Precision = aveprec / num_instance;
	return std::make_tuple(Average_Precision);
}
