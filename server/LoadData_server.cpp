/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�N���w���|�W�� txt �ɸ��J�ܵ{�����ñN�ɮפ����x�}�}�C(�ƾ� => ��ƶ��B�v��)�Ǵ�����ƫ��A MatrixXd 
Input Parameters
        ----------
        path                       : string    ; ���J�ƾڪ����| 
        EX 1 : "../../../Dataset/yale64x64_data_x_lin.txt";
        EX 2 : ""..\\weightPoolPath\\RASP2SERVER_0_1_GWeight_matrix.txt";
 Output Parameters
        ----------
        Eigen_original_data        : array     ; ��ƫ��A MatrixXd 
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
//#include "rtwtypes.h"
#include "LoadData_server.h"
using namespace Eigen;
using namespace std;


std::tuple<MatrixXd>  LoadData_server(string path)
{
    ifstream fin(path);
    string line_info;
    string input_result;

    vector<vector<double> > original_data;
    if (fin)
    {
        for (int i = 0; getline(fin, line_info); i++)
        {
            stringstream input(line_info);
            original_data.push_back(vector<double>());
            for (int j = 0; input >> input_result; ++j)
            {
                original_data[i].push_back(stod(input_result));
            }
        }
    }
    else
    {
        cout << "no such template points file" << endl;
    }
    MatrixXd Eigen_original_data(original_data.size(), original_data[0].size());
    for (int i = 0; i < original_data.size(); ++i)
    {
        Eigen_original_data.row(i) = VectorXd::Map(&original_data[i][0], original_data[0].size());
    }
    return std::make_tuple(Eigen_original_data);
}