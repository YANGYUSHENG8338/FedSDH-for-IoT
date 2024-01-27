/*
@author:     Yu - Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.1
@supervisor: Bo - Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    0.0.2
Log: 0.0.1 Initial
Log: 0.0.2 改成指標形式

***副程式用途***
查詢此資料夾是否清空，空資料夾不動作；資料夾有檔案清空
Input Parameters
        ----------
        weightPoolPath      : str&  ; 資料夾路徑
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
#include "EmptyWeightPool_client.h"
using namespace std;
using namespace Eigen;

#ifdef _WIN64 // Windows x64 環境下使用

#else// Linux 環境下使用
    bool createDirectory(const std::string& directoryPath)
    {
        int status = mkdir(directoryPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        return (status == 0);
    }
#endif


std::tuple<> EmptyWeightPool_client(const string& weightPoolPath)
{
    #ifdef _WIN64 // Windows x64 環境下使用
        cout << "最後一次電腦端遇到 POWER 和 MEMORY 直接跳過" << endl;
    #else// Linux 環境下使用

 
        // 檢查文件夾是否存在
        DIR* dir;
        dirent* pdir;

        // 要刪除檔案的資料夾路徑
        const char* folderPath  = weightPoolPath.c_str();

        // 開啟資料夾
        dir = opendir(folderPath);
        if (dir == NULL) {
            cout << "無法開啟資料夾 " << folderPath << endl;
            if (createDirectory(folderPath)) {
                        std::cout << "weightPoolPath 目錄創建成功！" << std::endl;
            } else {
                        std::cout << "無法創建 weightPoolPath 目錄。" << std::endl;
            }
        }


        // 遍歷資料夾中的所有檔案，刪除每個檔案
        while ((pdir = readdir(dir)) != NULL) {
            if (pdir->d_type == DT_REG) {
                string filePath = folderPath;
                filePath += "/";
                filePath += pdir->d_name;
                if (unlink(filePath.c_str()) != 0) {
                    cout << "刪除檔案 " << filePath << " 失敗" << endl;
                }
            }
        }

        // 關閉資料夾
        closedir(dir);
    #endif
    return std::make_tuple();
}





//std::tuple<> EmptyWeightPool_client(const string& weightPoolPath)
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
//
//    }
//    else {
//        cout << "文件夾 " << weightPoolPath << " 不存在，正在創建該資料集" << endl;
//        fs::create_directory(weightPoolPath);
//    }
//    return std::make_tuple();
//}

//// 檢查文件夾是否存在
//if (fs::exists(weightPoolPath))
//{
//    cout << "文件夾 " << weightPoolPath << " 存在。" << endl;
//    // 刪除文件夾中的所有文件
//    for (auto& file : fs::directory_iterator(weightPoolPath)) {
//        if (file.is_regular_file()) {
//            fs::remove(file);
//        }
//    }
//    cout << "文件夾 " << weightPoolPath << " 中的所有文件已刪除。" << endl;
//}
//else {
//    cout << "文件夾 " << weightPoolPath << " 不存在，正在創建該資料集" << endl;
//    fs::create_directory(weightPoolPath);
//}
