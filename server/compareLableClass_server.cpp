/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
��� cleint �ݶǦ^���������O�x�}�̭����������O���ơA�ñN�������O���X
Input Parameters
        ----------
        Lable1Class        : array  ; C_label1     - by - 1  ; �������O�x�} 1
        Lable2Class        : array  ; C_label2     - by - 1  ; �������O�x�} 2
 Output Parameters
        ----------
        commonValues       : array  ; commonValues - by - 1  ; ���Ƽ������O�x�}
*/
#include <stdio.h>
#include <iostream>
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
#include "compareLableClass_server.h"
using namespace std;
using namespace Eigen;

tuple< MatrixXd> compareLableClass_server(MatrixXd Lable1Class, MatrixXd Lable2Class)
{



    // �T�w�s�x�ۦP�Ʀr���x�}���̤p�j�p
    int minSize = std::min(Lable1Class.rows(), Lable2Class.rows());
    int maxSize = std::max(Lable1Class.rows(), Lable2Class.rows());
    MatrixXd commonValues(minSize, 1);              // �s�x�ۦP�Ʀr���x�}
    MatrixXd differentValues(maxSize - minSize, 1); // �s�x���P�Ʀr���x�}
    int commonCount = 0; // �ۦP�Ʀr���ƶq


    // �M��ۦP���Ʀr
    for (int Lable1ClassIdx = 0; Lable1ClassIdx < Lable1Class.rows(); Lable1ClassIdx++) {
        for (int Lable2ClassIdx = 0; Lable2ClassIdx < Lable2Class.rows(); Lable2ClassIdx++) {
            if (Lable1Class(Lable1ClassIdx, 0) == Lable2Class(Lable2ClassIdx, 0)) 
            {
                commonValues(commonCount, 0) = Lable1Class(Lable1ClassIdx, 0);
                commonCount++;
                break; // ���ۦP�Ʀr��A���X���h�j��
            }
        }
    }



    // ��X�ۦP���Ʀr
    std::cout << "�ۦP���Ʀr�G" << std::endl;
    for (int i = 0; i < commonCount; i++) {
        std::cout << commonValues(i, 0) << " ";
    }
    std::cout << std::endl;

    return std::make_tuple(commonValues);

}