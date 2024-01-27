QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += C++11


DEFINES += QT_DEPRECATED_WARNINGS



INCLUDEPATH += /usr/local/include/opencv4 #
INCLUDEPATH += ../include/extern #
INCLUDEPATH += ../include/eigen3 #

LIBS += -L/usr/lib/aarch64-linux-gnu -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_videoio


#openMP
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp


SOURCES += main_client.cpp
SOURCES += compactbit_client.cpp
SOURCES += dispMsg_client.cpp
SOURCES += double2txt_client.cpp
SOURCES += EmptyWeightPool_client.cpp
SOURCES += evaluate_client.cpp
SOURCES += FileExists_client.cpp
SOURCES += getAnchor_client.cpp
SOURCES += GetCurrentTimeStamp_client.cpp
SOURCES += getLabel_client.cpp
SOURCES += hammingDist_client.cpp
SOURCES += LoadData_client.cpp
SOURCES += meanstdPerAggreIter_client.cpp
SOURCES += meanstd_client.cpp
SOURCES += multilabelmaPrecision_client.cpp
SOURCES += PWeight_client.cpp
SOURCES += PWeightRRC_client.cpp
SOURCES += pythonRunner_client.cpp
SOURCES += RemoveEvaLogFile_client.cpp
SOURCES += SDH_client.cpp
SOURCES += SDHWrapper_client.cpp
SOURCES += sqdist_client.cpp
SOURCES += uniqLabels_client.cpp
SOURCES += WWeight_client.cpp
SOURCES += WWeightRRC_client.cpp

HEADERS += \
    main_client.h \
    compactbit_client.h \
    dispMsg_client.h \
    double2txt_client.h \
    EmptyWeightPool_client.h \
    evaluate_client.h \
    FileExists_client.h \
    getAnchor_client.h \
    GetCurrentTimeStamp_client.h \
    getLabel_client.h \
    hammingDist_client.h \
    LoadData_client.h \
    meanstdPerAggreIter_client.h \
    meanstd_client.h \
    multilabelmaPrecision_client.h \
    PWeight_client.h \
    PWeightRRC_client.h \
    pythonRunner_client.h \
    RemoveEvaLogFile_client.h \
    SDH_client.h \
    SDHWrapper_client.h \
    sqdist_client.h \
    uniqLabels_client.h \
    ../initpath.h \
    WWeight_client.h \
    WWeightRRC_client.h
