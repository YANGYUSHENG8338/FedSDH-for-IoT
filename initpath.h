#pragma once
#ifndef INITPATH_H
#define INITPATH_H

// Define the paths here

#define INIT_CLIENT_SERVER2                "/SERVER2"
#define INIT_CLIENT_Fed                    "_Fed"
#define INIT_CLIENT_EXP                    "_EXP"
#define INIT_CLIENT_ITR                    "_ITR"
#define INIT_CLIENT_B_SI                   "B_SI"
#define INIT_CLIENT_MU                     "_MU"
#define INIT_CLIENT_txt                    ".txt"
#define INIT_CLIENT_XtranMtxTxt            "_X_tran_matrix.txt"
#define INIT_CLIENT_XtuneMtxTxt            "_X_tune_matrix.txt"
#define INIT_CLIENT_XtestMtxTxt            "_X_test_matrix.txt"
#define INIT_CLIENT_LtranMtxTxt            "_L_tran_matrix.txt"
#define INIT_CLIENT_LtuneMtxTxt            "_L_tune_matrix.txt"
#define INIT_CLIENT_LtestMtxTxt            "_L_test_matrix.txt"
#define INIT_CLIENT_PWeightMtxTxt          "_PWeight_matrix.txt"
#define INIT_CLIENT_WWeightMtxTxt          "_WWeight_matrix.txt"
#define INIT_CLIENT_LabelListMtxTxt        "_LabelList_matrix.txt"
#define INIT_CLIENT_LabelCountMtxTxt       "_LabelCount_matrix.txt"
#define INIT_CLIENT_threadNamePath         "threadName.txt"
#define INIT_CLIENT_threadName             "./RaspClientDemo1"




#ifdef _WIN64 // Windows x64 環境下使用
// SERVER PATH 使用，接著因為 SERVER 端程式只在 x64上使用，所以只在此處定義巨集
	#define INIT_SERVER_weightPoolPath         "../weightPoolPath"
	#define INIT_SERVER_CTRL_MSG_1             "SERVER_SEND_TRAIN_TO_NODE"
	#define INIT_SERVER_CTRL_MSG_2             "NODE_SEND_WEIGHT_TO_SERVER"
	#define INIT_SERVER_CTRL_MSG_3             "SERVER_SEND_AGGRE_TO_NODE"
	#define INIT_SERVER_CTRL_MSG_4             "SERVER_SEND_SERVEREnd_TO_NODE"
	#define INIT_SERVER_CTRL_MSG_5             "NODE_SEND_ClientEnd_TO_SERVER"
	#define INIT_SERVER_SERVER2RASP            "/SERVER2RASP_"
	#define INIT_SERVER_SERVER2NANO            "/SERVER2NANO_"
	#define INIT_SERVER_XtranMtxTxt            "_X_tran_matrix.txt"
	#define INIT_SERVER_XtuneMtxTxt            "_X_tune_matrix.txt"
	#define INIT_SERVER_XtestMtxTxt            "_X_test_matrix.txt"
	#define INIT_SERVER_LtranMtxTxt            "_L_tran_matrix.txt"
	#define INIT_SERVER_LtuneMtxTxt            "_L_tune_matrix.txt"
	#define INIT_SERVER_LtestMtxTxt            "_L_test_matrix.txt"
	#define INIT_SERVER_RASP2SERVER            "/RASP2SERVER_"
	#define INIT_SERVER_NANO2SERVER            "/NANO2SERVER_"
	#define INIT_SERVER_PWeightMtxTxt          "_PWeight_matrix.txt"
	#define INIT_SERVER_WWeightMtxTxt          "_WWeight_matrix.txt"
	#define INIT_SERVER_LabelListMtxTxt        "_LabelList_matrix.txt"
	#define INIT_SERVER_LabelCountMtxTxt       "_LabelCount_matrix.txt"
	#define INIT_SERVER_ControlFile            "SEND_FILE_CTRL_MSG.txt"
	#define INIT_SERVER_Control2SERVERPath     "/mnt/c/Users/Dslab/Desktop/demo1/weightPoolPath/"
	#define INIT_SERVER_Control2RASPPath       "raspdslab@100.82.153.59:RaspClientDemo1/weightPoolPath/"
	#define INIT_SERVER_Control2NANOPath       "bn10513137@100.66.82.83:NANOClinetDemo1/weightPoolPath/"
	#define INIT_x64CLIENT_weightPoolPath      "../weightPoolPath"




#else // Linux 環境下使用

	#define INIT_RASPCLIENT_weightPoolPath    "../weightPoolPath" // raspdslab 為RaspberryOS使用者名稱
	#define INIT_NANOCLIENT_weightPoolPath    "../weightPoolPath" // bn10513137 為JetsonNanoOS使用者名稱
	#define INIT_RASPCLIENT_evaPoolPath    	  "../evaPoolPath/"   // raspdslab 為使用者名稱
	#define INIT_NANOCLIENT_evaPoolPath    	  "../evaPoolPath/"   // bn10513137 為使用者名稱


#endif
#endif // INITPATH_H
