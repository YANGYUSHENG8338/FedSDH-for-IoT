/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

	Input Parameters
		----------
		*Xtran    : array*    ; D - by - N ; �V�m���
		nAnch     : double    ; 0          ; ���I��

	Output Parameters
		----------
		*Anch     : array*    ; D - by - A ; ���I��Ưx�}(nAnch = 0������)
		*Anchidx  : array*    ; 1 - by - A ; ���I��ƫ���

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
	