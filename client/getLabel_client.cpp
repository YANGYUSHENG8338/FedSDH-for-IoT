/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1   Initial
Log: 0.0.2   �令���ЧΦ�

***�Ƶ{���γ~***
�N�w�����G�H "�벼�覡" �ഫ�����ҵ��G
Input Parameters
		----------
		Rpred              : *array  ; Ntran - by - Ntest    ; �V�m�˥��M���ռ˥���Hash code �O�_�ۦ�
		TrainLabel         : *vector ; Ntran - by - 1        ; �V�m�������ҵ��G; label encoding
	Output Parameters
		----------
		voteTable          : *array  ; C     - by - Ntest    ; ���Rpred�N���ո�ƹ�������ҵ��G�� "�벼��" �x�}           (voteTable)
		tpredMultHot       : *array  ; C     - by - Ntest    ; �O��{0,1}�զ��A��ܸ�column�����ո�ơA������row���ҵ��G    (tpredMultHot)
		tpredLabEnc        : *vector ; Ntest - by - 1        ; ���ո�ƪ����ҵ��Glabel encoding                            (tpredLabEnc)
		

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
#include "getLabel_client.h"
#include "uniqLabels_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;

std::tuple<MatrixXi, MatrixXi, MatrixXi> getLabel_client(MatrixXi* Rpred, MatrixXi* ttranLabelEnc, int LabelCount)//ttranLabelEnc
{
	int row    = Rpred->rows();
	int column = Rpred->cols();
	vector<int> tranLabelList      = std::get<0>(uniqLabels_client(ttranLabelEnc));
	int         labelClass         = tranLabelList.size();                                // �����`����
	MatrixXi*   voteTable          = new MatrixXi(MatrixXi::Zero(LabelCount, column));    // �إߧ벼�Ưx�}
	MatrixXi*   tpredMultHot       = new MatrixXi(MatrixXi::Zero(LabelCount, column));
	vector<Eigen::Index> idxs;
	// Vote �έp
	for (int dataIndex = 0; dataIndex < column; dataIndex++)
	{
		
		for (Index i = 0; i < Rpred->col(dataIndex).size(); i++)
		{
			if ((*Rpred)(i, dataIndex) == 1)
			{
				idxs.push_back(i);
			}
		}
		for (int j = 0; j < idxs.size(); j++) 
		{
			(*voteTable)((*ttranLabelEnc)(idxs[j], 0) - 1, dataIndex) = (*voteTable)((*ttranLabelEnc)(idxs[j], 0) - 1, dataIndex) + 1;
		}
		idxs.clear();
		
	}
	vector<int>     max_value;
	vector<int>     max_idx;
	MatrixXd::Index maxRow, maxCol;
	for (int m = 0; m < voteTable->cols(); m++)
	{
		int max = voteTable->col(m).maxCoeff(&maxRow, &maxCol);
		max_value.push_back(max);
		if (max > 0)
		{
			for (Index n = 0; n < voteTable->col(m).size(); n++)
			{
				if ((*voteTable)(n, m) == max)
				{
					max_idx.push_back(n);
				}
			}
		}
		else
		{
			max = 0;
			max_idx.push_back(0);
		}
	}
	//����Ҩϥ� tpredLabEnc(Label Encode Output)
	//Mat                            mat_max_idx = convertVector2Mat<int>(max_idx, 1, max_idx.size());
	//Matrix<int, Dynamic, Dynamic>* tpredLabEnc = new Matrix<int, Dynamic, Dynamic>();
	//cv2eigen(mat_max_idx, *tpredLabEnc);
	MatrixXi* tpredLabEnc      = new MatrixXi(MatrixXi::Zero(1, max_idx.size()));
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < max_idx.size(); j++)
		{
			(*tpredLabEnc)(i, j) = max_idx[j];
		}
	}

	for (int voteTable_col = 0; voteTable_col < voteTable->cols(); voteTable_col++)
	{
		for (int voteTable_row = 0; voteTable_row < voteTable->rows(); voteTable_row++)
		{
			if ((*voteTable)(voteTable_row, voteTable_col) == max_value[voteTable_col])
			{
				(*tpredMultHot)(voteTable_row, voteTable_col) = 1;
			}
		}
	}
	return std::make_tuple(*voteTable, *tpredMultHot, *tpredLabEnc);
}
