/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
將終端機所有輸出寫入一個文檔中
Input Parameters
        ----------
        disp2filePath          : str&       ; 檔案路徑
 Output Parameters
        ----------
        f                      : FILE*      ; // 文件指針
        coutbuf                : streambuf* ; // cout的緩衝區指針
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
    FILE* f                  = fopen(cOUTPUT_Path, "a+"); // 打開文件，文件指針為 f
    if (f == nullptr) {
        std::cout << "Error opening file." << std::endl;
    }
    // 打開一個輸出文件流，將輸出內容寫入到文件中
    std::ofstream outfile(cOUTPUT_Path);
    // 保存cout原有的緩衝區指針
    //std::streambuf* coutbuf = std::cout.rdbuf();
    //將cout的緩衝區指針重定向到outfile的緩衝區指針
    //std::cout.rdbuf(outfile.rdbuf());

    return std::make_tuple(f);
}


//std::tuple<FILE*, streambuf*> dispMsg_client(const string& disp2filePath)
//{
//    const char* cOUTPUT_Path = disp2filePath.c_str();
//    FILE* f = fopen(cOUTPUT_Path, "a+"); // 打開文件，文件指針為 f
//    if (f == nullptr) {
//        std::cout << "Error opening file." << std::endl;
//    }
//    // 打開一個輸出文件流，將輸出內容寫入到文件中
//    std::ofstream outfile(cOUTPUT_Path);
//    // 保存cout原有的緩衝區指針
//    std::streambuf* coutbuf = std::cout.rdbuf();
//    //將cout的緩衝區指針重定向到outfile的緩衝區指針
//    std::cout.rdbuf(outfile.rdbuf());
//
//    return std::make_tuple(f, coutbuf);
//}
