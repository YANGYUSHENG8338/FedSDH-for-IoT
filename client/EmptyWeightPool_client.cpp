/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�d�ߦ���Ƨ��O�_�M�šA�Ÿ�Ƨ����ʧ@�F��Ƨ����ɮײM��
Input Parameters
        ----------
        weightPoolPath      : str&  ; ��Ƨ����|
 Output Parameters
        ----------
        �L�Ǧ^��
*/
#ifdef _WIN64 // Windows x64 ���ҤU�ϥ�
    #include <windows.h>
    #include <filesystem>
#else// Linux ���ҤU�ϥ�
    #include <unistd.h>
    #include <sys/wait.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif
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
#include "EmptyWeightPool_client.h"
using namespace std;
using namespace Eigen;

#ifdef _WIN64 // Windows x64 ���ҤU�ϥ�

#else// Linux ���ҤU�ϥ�
    bool createDirectory(const std::string& directoryPath)
    {
        int status = mkdir(directoryPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        return (status == 0);
    }
#endif


std::tuple<> EmptyWeightPool_client(const string& weightPoolPath)
{
    #ifdef _WIN64 // Windows x64 ���ҤU�ϥ�
        cout << "�̫�@���q���ݹJ�� POWER �M MEMORY �������L" << endl;
    #else// Linux ���ҤU�ϥ�

 
        // �ˬd��󧨬O�_�s�b
        DIR* dir;
        dirent* pdir;

        // �n�R���ɮת���Ƨ����|
        const char* folderPath  = weightPoolPath.c_str();

        // �}�Ҹ�Ƨ�
        dir = opendir(folderPath);
        if (dir == NULL) {
            cout << "�L�k�}�Ҹ�Ƨ� " << folderPath << endl;
            if (createDirectory(folderPath)) {
                        std::cout << "weightPoolPath �ؿ��Ыئ��\�I" << std::endl;
            } else {
                        std::cout << "�L�k�Ы� weightPoolPath �ؿ��C" << std::endl;
            }
        }


        // �M����Ƨ������Ҧ��ɮסA�R���C���ɮ�
        while ((pdir = readdir(dir)) != NULL) {
            if (pdir->d_type == DT_REG) {
                string filePath = folderPath;
                filePath += "/";
                filePath += pdir->d_name;
                if (unlink(filePath.c_str()) != 0) {
                    cout << "�R���ɮ� " << filePath << " ����" << endl;
                }
            }
        }

        // ������Ƨ�
        closedir(dir);
    #endif
    return std::make_tuple();
}





//std::tuple<> EmptyWeightPool_client(const string& weightPoolPath)
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
//
//    }
//    else {
//        cout << "��� " << weightPoolPath << " ���s�b�A���b�ЫظӸ�ƶ�" << endl;
//        fs::create_directory(weightPoolPath);
//    }
//    return std::make_tuple();
//}

//// �ˬd��󧨬O�_�s�b
//if (fs::exists(weightPoolPath))
//{
//    cout << "��� " << weightPoolPath << " �s�b�C" << endl;
//    // �R����󧨤����Ҧ����
//    for (auto& file : fs::directory_iterator(weightPoolPath)) {
//        if (file.is_regular_file()) {
//            fs::remove(file);
//        }
//    }
//    cout << "��� " << weightPoolPath << " �����Ҧ����w�R���C" << endl;
//}
//else {
//    cout << "��� " << weightPoolPath << " ���s�b�A���b�ЫظӸ�ƶ�" << endl;
//    fs::create_directory(weightPoolPath);
//}
