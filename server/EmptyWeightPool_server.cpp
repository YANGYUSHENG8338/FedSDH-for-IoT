/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
查詢此資料夾是否清空，空資料夾不動作；資料夾有檔案清空
Input Parameters
        ----------
        weightPoolPath      : str&  ; 檔案路徑
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
#include <Eigen/Core>
#include <Eigen/Dense>
#include "EmptyWeightPool_server.h"
using namespace std;
using namespace Eigen;
namespace fs = std::filesystem;

std::tuple<> EmptyWeightPool_server(const string& weightPoolPath)
{
    try {
        if (fs::exists(weightPoolPath)) {
            cout << "文件夾 " << weightPoolPath << " 存在。" << endl;
            // 刪除文件夾中的所有文件
            for (const auto& file : fs::directory_iterator(weightPoolPath)) {
                if (file.is_regular_file()) {
                    fs::remove(file.path());
                    std::cout << "成功刪除檔案：" << file.path() << std::endl;
                }
            }
            cout << "文件夾 " << weightPoolPath << " 中的所有文件已刪除。" << endl;
        }
        else {
            cout << "文件夾 " << weightPoolPath << " 不存在，正在創建該資料夾。" << endl;
            fs::create_directory(weightPoolPath);
        }
    }
    catch (const std::filesystem::filesystem_error& ex) {
        // 處理例外
        std::cerr << "檔案系統錯誤: " << ex.what() << std::endl;
        // 可以加入其他錯誤處理邏輯，例如回報錯誤給使用者或重新嘗試操作
        // 刪除所有檔案及資料夾
        fs::remove_all(weightPoolPath);
        std::cout << "成功刪除資料夾及其內容：" << weightPoolPath << std::endl;
        //在創建一個 資料夾 weightPool
        fs::create_directory(weightPoolPath);
        cout << "文件夾 " << weightPoolPath << " 該資料集創建成功" << endl;
    }
    return std::make_tuple();
}
//std::tuple<> EmptyWeightPool_server(const string& weightPoolPath)
//{
//    // 檢查文件夾是否存在
//    if (fs::exists(weightPoolPath))
//    {
//        cout << "文件夾 " << weightPoolPath << " 存在。" << endl;
//        // 刪除文件夾中的所有文件
//        try {
//            for (const auto& file : fs::directory_iterator(weightPoolPath)) {
//                if (file.is_regular_file()) {
//                    fs::remove(file.path());
//                    std::cout << "成功刪除檔案：" << file.path() << std::endl;
//                }
//            }
//        }
//        catch (const std::filesystem::filesystem_error& ex) {
//            std::cerr << "刪除資料夾錯誤: " << ex.what() << std::endl;
//            // 可以加入其他錯誤處理邏輯，例如回報錯誤給使用者
//            // 刪除所有檔案及資料夾
//            fs::remove_all(weightPoolPath);
//            std::cout << "成功刪除資料夾及其內容：" << weightPoolPath << std::endl;
//            //在創建一個 資料夾 weightPool
//            fs::create_directory(weightPoolPath);
//            cout << "文件夾 " << weightPoolPath << " 該資料集創建成功" << endl;
//        }
//    }
//    else {
//        cout << "文件夾 " << weightPoolPath << " 不存在，正在創建該資料集" << endl;
//        fs::create_directory(weightPoolPath);
//    }
//    return std::make_tuple();
//}