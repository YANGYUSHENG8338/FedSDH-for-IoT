/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
擷取當下時間戳印，並印製在終端機上
Input Parameters
        ----------
		time_stamp_type     : int   ; 選取時間戳印到毫秒、微秒還是奈秒， 1 => 毫秒；2 => 微秒；3 => 奈秒
Output Parameters
		----------
		ss.str()            : str   ; 時間戳印 ex：2023-06-05 17:26:53:852:880:700


*/
#pragma warning( disable : 4996 )
#include <stdio.h>
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
//#include "rtwtypes.h"
#include "GetCurrentTimeStamp_server.h"
using namespace Eigen;
using namespace std;
using namespace std::chrono; // nanoseconds, system_clock, seconds


std::tuple<string> GetCurrentTimeStamp_server(int time_stamp_type)
{
    std::chrono::system_clock::time_point now        = std::chrono::system_clock::now();
    std::time_t                           now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm*                              now_tm     = std::localtime(&now_time_t);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%F %T", now_tm);
    std::ostringstream ss;
    ss.fill('0');
    std::chrono::milliseconds ms;
    std::chrono::microseconds cs;
    std::chrono::nanoseconds  ns;

    //ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    //cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
    //ns = std::chrono::duration_cast<std::chrono::nanoseconds >(now.time_since_epoch())  % 1000000000;
    switch (time_stamp_type)
    {
    case 0:
        ss << buffer;
        break;
    case 1:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        ss << buffer << ":" << ms.count();
        break;
    case 2:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000;
        break;
    case 3:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        ns = std::chrono::duration_cast<std::chrono::nanoseconds> (now.time_since_epoch()) % 1000000000;
        ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000 << ":" << ns.count() % 1000;
        break;
    default:
        ss << buffer;
        break;
    }
    return std::make_tuple(ss.str());
}