#pragma once
#ifndef SDH_CLIENT_H
#define SDH_CLIENT_H
// Include files
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm> 
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "SDH_client.h"
#include "LoadData_client.h"
#include "WWeight_client.h"
#include "PWeight_client.h"
#include "WWeightRRC_client.h"
#include "PWeightRRC_client.h"
#include "double2txt_client.h"
#include "uniqLabels_client.h"
#include "GetCurrentTimeStamp_client.h"
#include "../initpath.h"
using namespace Eigen;
using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

std::tuple<
           MatrixXd*, MatrixXd*, MatrixXi*, 
           std::chrono::high_resolution_clock::time_point, 
		   std::chrono::high_resolution_clock::time_point
		  > 
SDH_client(
           MatrixXd* X   , MatrixXi* y        , MatrixXi* B        , 
           double Prox_Mu, double Gmap_Lambda , string Gmap_loss   , string Fmap_type, double Fmap_Nu, double Fmap_Lambda, 
		   double tol    , 
		   int maxItr    , int debug          , int exp            , int itr         , 
		   MatrixXd* BPre, string SwitchMethod, string LoadDataPath, int LabelCount
		   );
#endif
