/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
���μ˥���ƶ��μ��Ҷ�
Input Parameters
		----------
		datasetnam           : str                         ; ���դ���ƶ��W��
		X                    : array  ; D     - by - N     ; ���B�z���
		Eigen_Label          : array  ; D     - by - N     ; ���Ҹ��
		XtranRatio           : double                      ; ���t�� Xtran �����
		XtuneRatio           : double                      ; ���t�� Xtune �����
		XtestRatio           : double                      ; ���t�� Xtest �����
 Output Parameters
		----------
		Xtran                : array  ; D     - by - Ntran ; �V�m��Ưx�}
		Xtune                : array  ; D     - by - Ntune ; �L�ո�Ưx�}
		Xtest                : array  ; D     - by - Ntest ; ���ո�Ưx�}
		idxtran              : array  ; Ntran - by - 1     ; �V�m��ƫ��Яx�}
		idxtune              : array  ; Ntune - by - 1     ; ���ո�ƫ��Яx�}
		idxtest              : array  ; Ntest - by - 1     ; ���ո�ƫ��Яx�}
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
	double   numtran, numtune, numtest;                                                                        // �N��ƶ����� Tran , Tune �M Test �o�T�����A�e���ܼƥN��U�۪��˥��ƥ�
	vector<int> init_idxtran, init_idxtune, init_idxtest, final_idxtest, randomVector;                         // init => �N���l���ү��ޭ� ; final => �N��B�z����ү��ޭ� ;
	numtran                                   = convertToEven(round(X.cols() * XtranRatio));                   // �V�m��ƭӼ�
	numtune                                   = convertToEven(round(X.cols() * XtuneRatio));                   // �L�ո�ƭӼ�
	numtest                                   = X.cols() - numtran - numtune;                                  // ���ո�ƭӼ�
	MatrixXd                 idxtran          = MatrixXd::Zero(1, numtran);                                    // �V�m��Ư��ޭ�
	MatrixXd                 idxtune          = MatrixXd::Zero(1, numtune);                                    // �L�ո�Ư��ޭ�
	MatrixXd                 idxtest;                                                                          // ���ո�Ư��ޭ�
	MatrixXd                 Xtran            = MatrixXd::Zero(X.rows(), numtran);                             // �V�m���
	MatrixXd                 Xtune            = MatrixXd::Zero(X.rows(), numtune);                             // �L�ո��
	MatrixXd                 Xtest;                                                                            // ���ո��

	// �N Label �p�ƨ�j
	auto                     uniqLabels_data  = uniqLabels_server(Eigen_Label);
	vector<double>           Label_uni        = std::get<0>(uniqLabels_data);
	vector<double>::iterator ip               = std::get<1>(uniqLabels_data);
	int                      LabelCount       = Label_uni.size();                                              // �������e����ƾڶ����X�ؼ���
	
	// Displaying the vector after applying std::unique 
	//// ��X Label �C���Ӽ� 
	//for (ip = Label_uni.begin(); ip != Label_uni.end(); ++ip)
	//{
	//	LabelNumber++;
	//}
	
	vector<int> idx;
	for (int m = 0; m < Eigen_Label.rows(); m++)
	{
		idx.push_back(m);
	}
	// �üƥ��èC�����O�����ޭ�
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
	// YALE �N test �h�ƻs�@��(15�Ӽ˥�)   => ��]�O�̷Ӥ�Ҥ��t�Atest �̲ץu����t�� 15 ���˥��ƥءA�p�G�N�o 15 �����ռ˥�������Ӹ˸m�|�y�����t���������D�A�]���N�����ռ˥��A�h�ƻs�@���A���ȯ����{�˥��ƪ����ä��t�A�ٯണ�ɸӸ˸m���������СC 






	// �N Vector �ഫ�� Matrix
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

	//YALE�ϥ�
	if (datasetnam.compare("YALE") == 0)
	{
		Xtest = MatrixXd::Zero(X.rows(), numtest * 2);
	}
	else
	{
		Xtest = MatrixXd::Zero(X.rows(), numtest);
	}

	// �N Label ���ޭȹ���������ഫ�� Xtran , Xtune �M Xtest
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

	return make_tuple(Xtran, Xtune, Xtest, idxtran, idxtune, idxtest, LabelCount); //LabelCount==�������e����ƾڶ����X�ؼ���
}
