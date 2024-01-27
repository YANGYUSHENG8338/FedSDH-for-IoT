/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
將控制指令(發送檔案至NODE)傳送到指定路徑上的控制檔案中
Input Parameters
        ----------
        SendOrReturnWeight                   : string&    ; 載入控制指令
 Output Parameters
        ----------
        無傳回值
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
#include <chrono>
#include <thread>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "SendOrReturnWeight_server.h"
#include "../initpath.h"
using namespace std;
using namespace Eigen;


std::tuple<> SendOrReturnWeight_server(const string& SendOrReturnWeight)
{
    // 發送檔案至NODE
    vector<string> control_strings = { SendOrReturnWeight };
    ofstream       control_file(INIT_SERVER_ControlFile, ios::out | ios::trunc);
    // 將指定檔案打開
    if (control_file.is_open()) {
        for (const auto& str : control_strings) {
            control_file << str << '\n';                          // 將每一個字串寫入一行
        }
        control_file.close();                                     // Close the file
        cout << SendOrReturnWeight << endl;
    }
    else {
        cout << "File creation failed." << endl;                  // 創建失敗
    }
    // 休眠 6000 毫秒(第四次) => 等待伺服端傳送檔案到客戶端
    auto now_4         = std::chrono::steady_clock::now();
    auto sleep_until_4 = now_4 + std::chrono::milliseconds(3000);
    std::this_thread::sleep_until(sleep_until_4);
    return std::make_tuple();
}
