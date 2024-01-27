#pragma once
#ifndef SDHWRAPPER_CLIENT_H
#define SDHWRAPPER_CLIENT_H
// Include files
#ifdef _WIN64 // Windows x64 環境下使用
	#include <windows.h>
	#include <filesystem>
#else// Linux 環境下使用
	#include <unistd.h>
	#include <sys/wait.h>
	#include <dirent.h>
#endif
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <tuple>
#include <ctime>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "SDH_client.h"
#include "sqdist_client.h"
#include "compactbit_client.h"
#include "hammingDist_client.h"

using namespace std;
using namespace Eigen;
using namespace cv;

std::tuple<
             MatrixXi*                                     , MatrixXd*                                     , MatrixXd*                                     ,
			 std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point, 
			 std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point 
		  >  
SDHWrapper_client
          ( MatrixXd* Xtran    , MatrixXd* Xtest    , MatrixXi* ttranLabelEnc, MatrixXi* ttestLabelEnc, MatrixXd* Anchor   , 
		    int L              , int LocalIteration , 
		    string SDH_Function, double SDH_RBFsigma, int SDH_HammingDist    , double SDH_Mu          , double SDH_FmapNu  , double  SDH_GmapLambda, double  SDH_FmapLambda,   
		    int exp            , int itr            , MatrixXd* Bpre         , 
		    string SwitchDevice, string SwitchMethod, string SwitchSegment   , string SwitchEvaluation, string LoadDataPath, string evaPoolPath    , int LabelCount
		   );
#endif
