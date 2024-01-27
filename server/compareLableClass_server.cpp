/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
比較 cleint 端傳回的標籤類別矩陣裡面有哪些類別重複，並將重複類別取出
Input Parameters
        ----------
        Lable1Class        : array  ; C_label1     - by - 1  ; 標籤類別矩陣 1
        Lable2Class        : array  ; C_label2     - by - 1  ; 標籤類別矩陣 2
 Output Parameters
        ----------
        commonValues       : array  ; commonValues - by - 1  ; 重複標籤類別矩陣
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



    // 確定存儲相同數字的矩陣的最小大小
    int minSize = std::min(Lable1Class.rows(), Lable2Class.rows());
    int maxSize = std::max(Lable1Class.rows(), Lable2Class.rows());
    MatrixXd commonValues(minSize, 1);              // 存儲相同數字的矩陣
    MatrixXd differentValues(maxSize - minSize, 1); // 存儲不同數字的矩陣
    int commonCount = 0; // 相同數字的數量


    // 尋找相同的數字
    for (int Lable1ClassIdx = 0; Lable1ClassIdx < Lable1Class.rows(); Lable1ClassIdx++) {
        for (int Lable2ClassIdx = 0; Lable2ClassIdx < Lable2Class.rows(); Lable2ClassIdx++) {
            if (Lable1Class(Lable1ClassIdx, 0) == Lable2Class(Lable2ClassIdx, 0)) 
            {
                commonValues(commonCount, 0) = Lable1Class(Lable1ClassIdx, 0);
                commonCount++;
                break; // 找到相同數字後，跳出內層迴圈
            }
        }
    }



    // 輸出相同的數字
    std::cout << "相同的數字：" << std::endl;
    for (int i = 0; i < commonCount; i++) {
        std::cout << commonValues(i, 0) << " ";
    }
    std::cout << std::endl;

    return std::make_tuple(commonValues);

}