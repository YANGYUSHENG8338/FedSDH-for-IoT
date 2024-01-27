/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
檔案是否存在，存在傳回 True；不存在傳回 False
Input Parameters
        ----------
        path          : str&  ; 檔案路徑 // @@@ fileName => path 暴力搜尋其他檔案
 Output Parameters
        ----------
        file.good()   : bool  ; 回傳 True 存在 ; 回傳 False 不存在
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
