/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 �令���ЧΦ�

***�Ƶ{���γ~***
�R���e�@�Ӵ��սX���d�U�� Power �M Memory �� �����ɮ� (logOneAggrePower.txt , logTrainPower.txt, logTestPower.txt, logOneAggreMemory.txt , logTrainMemory.txt, logTestMemory.txt) �M (SumlogOneAggrePower.txt , SumlogTrainPower.txt, SumlogTestPower.txt, SumlogOneAggreMemory.txt , SumlogTrainMemory.txt, SumlogTestMemory.txt)
Input Parameters
		----------
		evaPoolPath      : str&  ; �ɮ׸��|
		SwitchSegment    : str&  ; ��ܶq������@�^�ҫ���s�Ϭq�άO�b�ҫ��V�m�μҫ����հϬq�i��(ALL �M TRAINTEST)
		SwitchEvaluation : str&  ; ��ܶq����ص�������  (SPEED, POWER �M MEMORY)
 Output Parameters
		----------
		�L�Ǧ^��
*/
#ifdef _WIN64 // Windows x64 ���ҤU�ϥ�
    #include <windows.h>
    #include <filesystem>
#else// Linux ���ҤU�ϥ�
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
#ifdef _WIN64 // Windows x64 ���ҤU�ϥ�

#else// Linux ���ҤU�ϥ�
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
#ifdef _WIN64 // Windows x64 ���ҤU�ϥ�
	cout << "�q���ݹJ�� POWER �M MEMORY �������L" << endl;
#else// Linux ���ҤU�ϥ�
    if (!directoryExists(evaPoolPath))
    {
        if (createDirectory1(evaPoolPath)) {
            std::cout << "�ؿ��Ыئ��\�I" << std::endl;
        } else {
            std::cout << "�L�k�Ыإؿ��C" << std::endl;
        }
    }
    else
    {
        std::cout << "�ؿ��w�s�b�C" << std::endl;
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

