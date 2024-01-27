#pragma once
#ifndef UNIQLABELS_CLIENT_H
#define UNIQLABELS_CLIENT_H
// Include files
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cmath>
#include <iterator>
#include <random>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
using namespace std;
using namespace Eigen;
using namespace cv;


tuple<vector<int>, vector<int>::iterator > uniqLabels_client(MatrixXi* MatrixXi_Label);

#endif