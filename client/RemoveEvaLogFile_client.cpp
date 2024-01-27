/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
刪除前一個測試碼長留下的 Power 和 Memory 的 紀錄檔案 (logOneAggrePower.txt , logTrainPower.txt, logTestPower.txt, logOneAggreMemory.txt , logTrainMemory.txt, logTestMemory.txt) 和 (SumlogOneAggrePower.txt , SumlogTrainPower.txt, SumlogTestPower.txt, SumlogOneAggreMemory.txt , SumlogTrainMemory.txt, SumlogTestMemory.txt)
Input Parameters
		----------
		evaPoolPath      : str&  ; 檔案路徑
		SwitchSegment    : str&  ; 選擇量測完整一回模型更新區段或是在模型訓練或模型測試區段進行(ALL 和 TRAINTEST)
		SwitchEvaluation : str&  ; 選擇量測何種評估指標  (SPEED, POWER 和 MEMORY)
 Output Parameters
		----------
		無傳回值
*/
#ifdef _WIN64 // Windows x64 環境下使用
    #include <windows.h>
    #include <filesystem>
#else// Linux 環境下使用
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
#include "RemoveEvaLogFile_client.h"
using namespace std;
using namespace Eigen;
#ifdef _WIN64 // Windows x64 環境下使用

#else// Linux 環境下使用
    bool directoryExists(const std::string& directoryPath)
    {
        struct stat buffer;
        return (stat(directoryPath.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
    }
    bool createDirectory1(const std::string& directoryPath)
    {
        int status = mkdir(directoryPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        return (status == 0);
    }
#endif


std::tuple<> RemoveEvaLogFile_client(const string& evaPoolPath, const string& SwitchSegment, const string& SwitchEvaluation)
{
#ifdef _WIN64 // Windows x64 環境下使用
	cout << "電腦端遇到 POWER 和 MEMORY 直接跳過" << endl;
#else// Linux 環境下使用
    if (!directoryExists(evaPoolPath))
    {
        if (createDirectory1(evaPoolPath)) {
            std::cout << "目錄創建成功！" << std::endl;
        } else {
            std::cout << "無法創建目錄。" << std::endl;
        }
    }
    else
    {
        std::cout << "目錄已存在。" << std::endl;
    }
	int RemoveFile1Status;
	int RemoveFile2Status;
	int RemoveFile3Status;
	int RemoveFile4Status;
	if (SwitchSegment.compare("ALL") == 0)
	{
		if (SwitchEvaluation.compare("POWER") == 0)
		{
			RemoveFile1Status = std::remove((evaPoolPath + "logOneAggrePower.txt"   ).c_str());
			RemoveFile2Status = std::remove((evaPoolPath + "SumlogOneAggrePower.txt").c_str());
		}
		else if (SwitchEvaluation.compare("MEMORY") == 0)
		{
			RemoveFile1Status = std::remove((evaPoolPath + "logOneAggreMemory.txt"   ).c_str());
			RemoveFile2Status = std::remove((evaPoolPath + "SumlogOneAggreMemory.txt").c_str());

		}
		if (RemoveFile1Status != 0) {
			std::perror("Error deleting logOneAggreFile");
		}
		else
		{
			std::puts("logOneAggreFile deleted successfully");
		}
	}
	else if (SwitchSegment.compare("TRAINTEST") == 0)
	{
		if (SwitchEvaluation.compare("POWER") == 0)
		{
			RemoveFile1Status = std::remove((evaPoolPath + "logTrainPower.txt"   ).c_str());
			RemoveFile2Status = std::remove((evaPoolPath + "logTestPower.txt"    ).c_str());
			RemoveFile3Status = std::remove((evaPoolPath + "SumlogTrainPower.txt").c_str());
			RemoveFile4Status = std::remove((evaPoolPath + "SumlogTestPower.txt" ).c_str());

		}
		else if (SwitchEvaluation.compare("MEMORY") == 0)
		{
			RemoveFile1Status = std::remove((evaPoolPath + "logTrainMemory.txt"   ).c_str());
			RemoveFile2Status = std::remove((evaPoolPath + "logTestMemory.txt"    ).c_str());
			RemoveFile3Status = std::remove((evaPoolPath + "SumlogTrainMemory.txt").c_str());
			RemoveFile4Status = std::remove((evaPoolPath + "SumlogTestMemory.txt" ).c_str());

		}
		if (RemoveFile1Status != 0) {
			std::perror("Error deleting logTrainFile");
		}
		else
		{
			std::puts("logTrainFile deleted successfully");
		}
		if (RemoveFile2Status != 0) {
			std::perror("Error deleting logTestFile");
		}
		else
		{
			std::puts("logTestFile deleted successfully");
		}
	}

#endif
	return std::make_tuple();
}

