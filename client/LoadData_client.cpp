/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
將指定路徑上的 txt 檔載入至程式中並將檔案中的矩陣陣列(數據 => 資料集、權重)傳換為資料型態 MatrixXd
Input Parameters
        ----------
        path                       : string    ; 載入數據的路徑 // @@@filename 改成 path ，加上路徑名稱(例子)
        EX 1 : "../../../Dataset/yale64x64_data_x_lin.txt";
        EX 2 : ""..\\weightPoolPath\\RASP2SERVER_0_1_GWeight_matrix.txt";
 Output Parameters
        ----------
        Eigen_original_data        : array     ; 資料型態 MatrixXd
*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "LoadData_client.h"
using namespace Eigen;
using namespace std;

std::tuple<MatrixXd>  LoadData_client(string path)
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

//std::tuple<MatrixXd*>  LoadData_client (string filename)
//{
//    ifstream* fin_ptr = new ifstream(filename);
//    string* line_info_ptr = new string();
//    string* input_result_ptr = new string();
//
//    vector<vector<double> >* original_data_ptr = new vector<vector<double> >();
//    if (*fin_ptr)
//    {
//        for (int i = 0; getline(*fin_ptr, *line_info_ptr); i++)
//        {
//            stringstream input(*line_info_ptr);
//            original_data_ptr->push_back(vector<double>());
//            for (int j = 0; input >> *input_result_ptr; ++j)
//            {
//                original_data_ptr->at(i).push_back(stod(*input_result_ptr));
//            }
//        }
//    }
//    else
//    {
//        cout << "no such template points file" << endl;
//    }
//
//    MatrixXd* Eigen_original_data_ptr = new MatrixXd(original_data_ptr->size(), original_data_ptr->at(0).size());
//    for (int i = 0; i < original_data_ptr->size(); ++i)
//    {
//        (*Eigen_original_data_ptr).row(i) = VectorXd::Map(&(*original_data_ptr)[i][0], original_data_ptr->at(0).size());
//    }
//    //return std::make_tuple(Eigen_original_data);
//    return std::make_tuple(Eigen_original_data_ptr);
//}