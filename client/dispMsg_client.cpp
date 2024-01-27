/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�N�׺ݾ��Ҧ���X�g�J�@�Ӥ��ɤ�
Input Parameters
        ----------
        disp2filePath          : str&       ; �ɮ׸��|
 Output Parameters
        ----------
        f                      : FILE*      ; // �����w
        coutbuf                : streambuf* ; // cout���w�İϫ��w
*/
#pragma warning( disable : 4996 )
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
#include "dispMsg_client.h"
using namespace std;
using namespace Eigen;

std::tuple<FILE*> dispMsg_client(const string& disp2filePath)
{
    const char* cOUTPUT_Path = disp2filePath.c_str();
    FILE* f                  = fopen(cOUTPUT_Path, "a+"); // ���}���A�����w�� f
    if (f == nullptr) {
        std::cout << "Error opening file." << std::endl;
    }
    // ���}�@�ӿ�X���y�A�N��X���e�g�J����
    std::ofstream outfile(cOUTPUT_Path);
    // �O�scout�즳���w�İϫ��w
    //std::streambuf* coutbuf = std::cout.rdbuf();
    //�Ncout���w�İϫ��w���w�V��outfile���w�İϫ��w
    //std::cout.rdbuf(outfile.rdbuf());

    return std::make_tuple(f);
}


//std::tuple<FILE*, streambuf*> dispMsg_client(const string& disp2filePath)
//{
//    const char* cOUTPUT_Path = disp2filePath.c_str();
//    FILE* f = fopen(cOUTPUT_Path, "a+"); // ���}���A�����w�� f
//    if (f == nullptr) {
//        std::cout << "Error opening file." << std::endl;
//    }
//    // ���}�@�ӿ�X���y�A�N��X���e�g�J����
//    std::ofstream outfile(cOUTPUT_Path);
//    // �O�scout�즳���w�İϫ��w
//    std::streambuf* coutbuf = std::cout.rdbuf();
//    //�Ncout���w�İϫ��w���w�V��outfile���w�İϫ��w
//    std::cout.rdbuf(outfile.rdbuf());
//
//    return std::make_tuple(f, coutbuf);
//}
