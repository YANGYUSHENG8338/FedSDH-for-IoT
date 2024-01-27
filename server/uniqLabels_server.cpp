/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�h�� vector �}�C�̭��ƪ��ƭȡA�ñN�ƭȥѤp�ƨ�j�A�ت��O�� Label ���������X��
Input Parameters
        ----------
        Eigen_Label                   : string&             ; ���J������O
 Output Parameters
        ----------
        Label_uni                     : array  ; 1 - by - C ; �V�m��Ưx�}
        ip                            : array  ; 1 - by - C ; �L�ո�Ưx�}
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
using namespace std;
using namespace Eigen;
//### �ƪ�
tuple<vector<double>, vector<double>::iterator > uniqLabels_server(VectorXd Eigen_Label) 
{
    vector<double>           Label;
    vector<double>::iterator ip;
    Label.resize(Eigen_Label.size());
    VectorXd::Map(&Label[0], Eigen_Label.size()) = Eigen_Label;
    vector<double>           Label_uni(Label.begin(), Label.end());

    // Sorting the array 
    sort(Label_uni.begin(), Label_uni.end());
    // ��X Label ���O��
    ip = unique(Label_uni.begin(), Label_uni.begin() + Eigen_Label.rows());
    Label_uni.resize(std::distance(Label_uni.begin(), ip));
    return std::make_tuple(Label_uni, ip);

}