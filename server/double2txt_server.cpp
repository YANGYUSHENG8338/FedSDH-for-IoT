/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�N���o�� Data (double)�x�s��S�w���|�U�åH txt ���x�s
Input Parameters
        ----------
        mat        : array  ; D - by - double ��Ưx�}
        path       : array  ; �x�s���|
        EX 1 : "..\\weightPoolPath\\SERVER2RASP_0_X_tran_matrix.txt";
        EX 2 : "..\\weightPoolPath\\SERVER2NANO_0_1_GWeight_matrix.txt";
 Output Parameters
        ----------
        �L�Ǧ^��
*/
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "double2txt_server.h"
using namespace std;
using namespace Eigen;

std::tuple< > double2txt_server(Eigen::MatrixXd mat, string path)
{
    ofstream outfile(path, ios::trunc);
    outfile << mat << std::endl;;
    outfile.close();
    return std::make_tuple();
}