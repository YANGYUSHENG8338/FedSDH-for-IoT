/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

	Input Parameters
		----------
		*Xtran    : array*    ; D - by - N ; 訓練資料
		nAnch     : double    ; 0          ; 錨點數

	Output Parameters
		----------
		*Anch     : array*    ; D - by - A ; 錨點資料矩陣(nAnch = 0取全部)
		*Anchidx  : array*    ; 1 - by - A ; 錨點資料指標

*/
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "getAnchor_client.h"
using namespace Eigen;
using namespace std;
tuple<MatrixXd*, MatrixXd*> getAnchor_client(MatrixXd* Xtran, double nAnch)
{
	if (nAnch > 0)
	{
		//cout << "***********************************************" << endl;
	}
	else
	{
		MatrixXd* Anch = Xtran;
		MatrixXd* Anchidx = new MatrixXd(MatrixXd::Zero((*Anch).rows(), (*Anch).cols()));
		for (int m = 0; m < (*Anch).rows(); m++)
		{
			for (int n = 0; n < (*Anch).cols(); n++)
			{
				(*Anchidx)(m, n) = n;
			}
		}
		return std::make_tuple(Anch, Anchidx);
	}
}
	