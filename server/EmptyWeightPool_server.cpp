/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�d�ߦ���Ƨ��O�_�M�šA�Ÿ�Ƨ����ʧ@�F��Ƨ����ɮײM��
Input Parameters
        ----------
        weightPoolPath      : str&  ; �ɮ׸��|
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
#include "EmptyWeightPool_server.h"
using namespace std;
using namespace Eigen;
namespace fs = std::filesystem;

std::tuple<> EmptyWeightPool_server(const string& weightPoolPath)
{
    try {
        if (fs::exists(weightPoolPath)) {
            cout << "��� " << weightPoolPath << " �s�b�C" << endl;
            // �R����󧨤����Ҧ����
            for (const auto& file : fs::directory_iterator(weightPoolPath)) {
                if (file.is_regular_file()) {
                    fs::remove(file.path());
                    std::cout << "���\�R���ɮסG" << file.path() << std::endl;
                }
            }
            cout << "��� " << weightPoolPath << " �����Ҧ����w�R���C" << endl;
        }
        else {
            cout << "��� " << weightPoolPath << " ���s�b�A���b�ЫظӸ�Ƨ��C" << endl;
            fs::create_directory(weightPoolPath);
        }
    }
    catch (const std::filesystem::filesystem_error& ex) {
        // �B�z�ҥ~
        std::cerr << "�ɮרt�ο��~: " << ex.what() << std::endl;
        // �i�H�[�J��L���~�B�z�޿�A�Ҧp�^�����~���ϥΪ̩έ��s���վާ@
        // �R���Ҧ��ɮפθ�Ƨ�
        fs::remove_all(weightPoolPath);
        std::cout << "���\�R����Ƨ��Ψ䤺�e�G" << weightPoolPath << std::endl;
        //�b�Ыؤ@�� ��Ƨ� weightPool
        fs::create_directory(weightPoolPath);
        cout << "��� " << weightPoolPath << " �Ӹ�ƶ��Ыئ��\" << endl;
    }
    return std::make_tuple();
}
//std::tuple<> EmptyWeightPool_server(const string& weightPoolPath)
//{
//    // �ˬd��󧨬O�_�s�b
//    if (fs::exists(weightPoolPath))
//    {
//        cout << "��� " << weightPoolPath << " �s�b�C" << endl;
//        // �R����󧨤����Ҧ����
//        try {
//            for (const auto& file : fs::directory_iterator(weightPoolPath)) {
//                if (file.is_regular_file()) {
//                    fs::remove(file.path());
//                    std::cout << "���\�R���ɮסG" << file.path() << std::endl;
//                }
//            }
//        }
//        catch (const std::filesystem::filesystem_error& ex) {
//            std::cerr << "�R����Ƨ����~: " << ex.what() << std::endl;
//            // �i�H�[�J��L���~�B�z�޿�A�Ҧp�^�����~���ϥΪ�
//            // �R���Ҧ��ɮפθ�Ƨ�
//            fs::remove_all(weightPoolPath);
//            std::cout << "���\�R����Ƨ��Ψ䤺�e�G" << weightPoolPath << std::endl;
//            //�b�Ыؤ@�� ��Ƨ� weightPool
//            fs::create_directory(weightPoolPath);
//            cout << "��� " << weightPoolPath << " �Ӹ�ƶ��Ыئ��\" << endl;
//        }
//    }
//    else {
//        cout << "��� " << weightPoolPath << " ���s�b�A���b�ЫظӸ�ƶ�" << endl;
//        fs::create_directory(weightPoolPath);
//    }
//    return std::make_tuple();
//}