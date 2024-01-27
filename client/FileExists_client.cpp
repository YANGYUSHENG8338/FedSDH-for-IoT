/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�ɮ׬O�_�s�b�A�s�b�Ǧ^ True�F���s�b�Ǧ^ False
Input Parameters
        ----------
        path          : str&  ; �ɮ׸��| // @@@ fileName => path �ɤO�j�M��L�ɮ�
 Output Parameters
        ----------
        file.good()   : bool  ; �^�� True �s�b ; �^�� False ���s�b
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
#include "FileExists_client.h"
using namespace std;
using namespace Eigen;


std::tuple<bool> FileExists_client(const string& path)
{
    ifstream file(path);
    return file.good();
}
