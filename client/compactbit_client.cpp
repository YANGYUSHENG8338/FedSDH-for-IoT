/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

Input Parameters
		----------
		b_ptr          : array ; nSample - by - L      ; // 原始矩陣
 Output Parameters
		----------
		cb_ptr         : array ; nSample - by - nwords ; // compact矩陣
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
#include "compactbit_client.h"
using namespace Eigen;
using namespace std;
using namespace cv;



std::tuple<MatrixXi*> compactbit_client(MatrixXi* b_ptr)
{
    int nSamples = b_ptr->rows();
    int nbits = b_ptr->cols();
    int times = nbits / 8;
    int nwords = ceil(nbits / 8);
    MatrixXi* cb_ptr = new MatrixXi(nSamples, nwords);
    int sum = 0;
    for (int m = 0; m < nSamples; m++)
    {
        int w = floor(nbits);
        for (int n = 0; n < times; n++)
        {
            for (int k = 0; k < 8; k++)
            {
                int col = n * 8 + k;
                int b_value = (*b_ptr)(m, col);
                int pow_value = pow(2, k);
                sum += (b_value * pow_value);
            }
            (*cb_ptr)(m, n) = sum;
            sum = 0;
        }
    }

    return std::make_tuple(cb_ptr);
    delete b_ptr;  // 釋放 b 的指標
    delete cb_ptr; // 釋放 cb 的指標
}
