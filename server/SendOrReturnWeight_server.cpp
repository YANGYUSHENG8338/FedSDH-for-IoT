/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�N������O(�o�e�ɮצ�NODE)�ǰe����w���|�W�������ɮפ�
Input Parameters
        ----------
        SendOrReturnWeight                   : string&    ; ���J������O
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
    // �o�e�ɮצ�NODE
    vector<string> control_strings = { SendOrReturnWeight };
    ofstream       control_file(INIT_SERVER_ControlFile, ios::out | ios::trunc);
    // �N���w�ɮץ��}
    if (control_file.is_open()) {
        for (const auto& str : control_strings) {
            control_file << str << '\n';                          // �N�C�@�Ӧr��g�J�@��
        }
        control_file.close();                                     // Close the file
        cout << SendOrReturnWeight << endl;
    }
    else {
        cout << "File creation failed." << endl;                  // �Ыإ���
    }
    // ��v 6000 �@��(�ĥ|��) => ���ݦ��A�ݶǰe�ɮר�Ȥ��
    auto now_4         = std::chrono::steady_clock::now();
    auto sleep_until_4 = now_4 + std::chrono::milliseconds(3000);
    std::this_thread::sleep_until(sleep_until_4);
    return std::make_tuple();
}
