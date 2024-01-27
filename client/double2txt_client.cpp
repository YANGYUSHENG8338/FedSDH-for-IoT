/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
將分發後 Data (double)儲存到特定路徑下並以 txt 檔儲存
Input Parameters
        ----------
        mat        : array  ; D - by - double 資料矩陣
        path       : array  ; 儲存路徑// 
        EX 1 : "..\\weightPoolPath\\NANO2SERVER_0_1_GWeight_matrix.txt";
 Output Parameters
        ----------
        無傳回值
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
#include "double2txt_client.h"
using namespace std;
using namespace Eigen;

std::tuple< > double2txt_client(Eigen::MatrixXd* mat, string path) //@@@ fileName = > path 暴力搜尋其他檔案
{
    ofstream outfile(path, ios::trunc);
    outfile << *mat << std::endl;;
    outfile.close();
    return std::make_tuple();
}