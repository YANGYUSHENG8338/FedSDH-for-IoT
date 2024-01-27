/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式
Input Parameters
        ----------
        *B1         : array* ; ndatapoints1 - by - nwords       ; 漢明距離矩陣1
        *B2         : array* ; ndatapoints2 - by - nwords       ; 漢明距離矩陣2
 Output Parameters
        ----------
        *Dh         : array* ; ndatapoints1 - by - ndatapoints2 ; 漢明距離
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
#include "hammingDist_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;



std::tuple<MatrixXi*> hammingDist_client(MatrixXi* B1, MatrixXi* B2)
{
    //loop - up table :
    MatrixXi bit_in_char(1,256);
    bit_in_char <<
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3,
        3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4,
        3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2,
        2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5,
        3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5,
        5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3,
        2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
        4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4,
        4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6,
        5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5,
        5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8;
    int n1 = B1->rows();
    int n2 = B2->rows();
    int nwords = B2->cols();
    MatrixXi* Dh = new MatrixXi(MatrixXi::Zero(n1, n2));
    MatrixXi y = MatrixXi::Zero(n2, 1);
    for (int j = 0; j < n1; j++)
    {
        for (int n = 0; n < nwords; n++)
        {
            for (int m = 0; m < n2; m++)
            {
                (*Dh)(j, m) += bit_in_char(0, (*B1)(j, n) ^ (*B2)(m, n));
            }
        }
    }
	return std::make_tuple(Dh);
}