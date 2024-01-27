/*
@author:     Yu-Sheng Yang   @NSYSU(email: m0093010024@g-mail.nsysu.edu.tw) - 0.0.2
@supervisor: Bo-Wei Chen     @NSYSU(email: bo - wei.chen@mail.nsysu.edu.tw)
@version:    
Log: 0.0.1 Initial
     0.0.2 �令���ЧΦ�
     0.0.3 FedProxSDH��Q����Bpre
	       FedProxFSDH��UpdateIBWeight����Bpre
	 0.0.4 sigma�MGmap_Lambda��double [BWC]
	 
	Input Parameters
		----------
		*X               : array*  ; Ntran - by - Ntran ; �V�m Phi ���
		*y               : array*  ; Ntran - by - 1     ; �V�m��Ƽ���
		*B               : array*  ; Ntran - by - L     ; ��l B ���
		Prox_Mu          : double  ; FedProx �����h�ưѼ�
		Gmap_Lambda      : double  ; G-STEP-lambda      �DWWeight��Ridge Param
		Gmap_loss        : str     ; L2
		Fmap_type        : str     ; SDH_Function
		Fmap_Nu          : double  ; SDH �����h�ưѼ�
		Fmap_Lambda      : double  ; F-STEP-lambda      �DPWeight��Ridge Param
		LocalIteration   : double  ; �����v����s����
		tol              : double  ; bias �Ѽ�
		exp              : int     ; ����X�^���㪺�ҫ���s
		itr              : int     ; �̤j���A�ݻE�X����
		*Bpre            : array*  ; Ntran - by - L     ; �e�@�E�X������ B �x�}
		SwitchMethod     : string  ; ��ܦb���Ӹ˸m�W�i���v����s(NANO , RASP)
		weightPoolPath   : string  ; ��m��s�v���θ�ƶ����@�ɸ�Ƨ�
	Output Parameters
		----------
		*WWeight                      : array*                ; L     - by - C ; G-STEP-�v��
		*PWeight                      : array*                ; Ntran - by - L ; F-STEP-�v��
		*B                            : array                 ; Ntran - by - L ; ��sB���
		start_WaitAggreWeight         : high_resolution_clock ; �������� Server �Ǧ^�E�X�v���}�l�ɶ��W
		end_WaitAggreWeight           : high_resolution_clock ; �������� Server �Ǧ^�E�X�v�������ɶ��W
*/
#pragma warning( disable : 4996 ) 
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


typedef struct args_struct {
	int LinFormWght;
	int QuadFormWght;
}argu;

void RemoveRow(MatrixXd* matrix, unsigned int rowToRemove) {
	unsigned int numRows = matrix->rows() - 1;
	unsigned int numCols = matrix->cols();

	if (rowToRemove < numRows) {
		matrix->block(rowToRemove, 0, numRows - rowToRemove, numCols) = matrix->block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);
	}

	matrix->conservativeResize(numRows, numCols);
}
void RemoveColumn(MatrixXd* matrix, unsigned int colToRemove) {
	unsigned int numRows = matrix->rows();
	unsigned int numCols = matrix->cols() - 1;

	if (colToRemove < numCols) {
		matrix->block(0, colToRemove, numRows, numCols - colToRemove) = matrix->block(0, colToRemove + 1, numRows, numCols - colToRemove);
	}

	matrix->conservativeResize(numRows, numCols);
}




std::tuple<
           MatrixXd*, MatrixXd*, MatrixXi*, 
		   std::chrono::high_resolution_clock::time_point, 
		   std::chrono::high_resolution_clock::time_point
		  > 
SDH_client
          (
           MatrixXd* X   , MatrixXi* y        , MatrixXi* B        , 
		   double Prox_Mu, double Gmap_Lambda , string Gmap_loss   , string Fmap_type, double Fmap_Nu, double Fmap_Lambda, 
		   double tol    , int LocalIteration , int debug          , int exp         , int itr       , 
		   MatrixXd* Bpre, string SwitchMethod, string LoadDataPath, int LabelCount
		  )
{
	// �ѼƳ]�w
	int         N              = B->rows();
	int         L              = B->cols();
	int         localitr       = 0;                               // ��l�Ƨ����v����s����
	//Proximal Term�g�@�Ѽ�
	double      mu             = Prox_Mu;                         // FedProx ���h�ưѼ�
	double      nu             = Fmap_Nu;                         // �g�@�Ѽ�
	double      detla          = 1 / nu;
	tol                        = 1e-7;                            // bias �Ѽ�
	string      str_EXP        = to_string(exp);                  // ����X�^���㪺�ҫ���s
	string      str_ITR        = to_string(itr);                  // �̤j���A�ݻE�X����

	//argument
	argu        args_value     = { 1, 0};
	double      linFormWght    = args_value.LinFormWght;
	int         quadFormWght   = args_value.QuadFormWght;
	MatrixXi    LinForm        = MatrixXi::Zero(N, 1);

	//ONE_HOT_ENCODING
	//Label matrix N x c
	auto        uniqLabelsData = uniqLabels_client(y);
	vector<int> uniqLabelList  = std::get<0>(uniqLabelsData);
	MatrixXi*   IyOneHotLabel  = new MatrixXi(MatrixXi::Zero(y->rows(), LabelCount));
	// �ѩ� Matlab ���_�l�Ȥ��P�A�ҥH�ݦ��� 1
	if (y->cols() == 1)
	{
		for (int i = 0; i < y->rows(); i++)
		{
			(*IyOneHotLabel)(i, (*y)(i, 0) - 1) = 1;
		}
	}
	else
	{
		*IyOneHotLabel = *y;
	}

	MatrixXd RenewLabelList = MatrixXd::Zero(uniqLabelList.size(), 1); // �Ыظm����Ҧ����X�����x�}
	// ���o�Ӹ˸m�o����Ҧ����X���A�ñN���J�x�}�� 
	for (int RenewLabelListIdx = 0; RenewLabelListIdx < uniqLabelList.size(); RenewLabelListIdx++)
	{
		RenewLabelList(RenewLabelListIdx, 0) = uniqLabelList[RenewLabelListIdx];
	}



	std::chrono::high_resolution_clock::time_point start_WaitAggreWeight, end_WaitAggreWeight;     // �ŧi�������� Server �Ǧ^�E�X�v�����ɶ��W
	Matrix<double, Dynamic, Dynamic>* WWeight    = new Matrix<double, Dynamic, Dynamic>();         // ������s�v�� ( WWeight )
	Matrix<double, Dynamic, Dynamic>* PWeight    = new Matrix<double, Dynamic, Dynamic>();         // ������s�v�� ( PWeight )
	Matrix<double, Dynamic, Dynamic>* WF0        = new Matrix<double, Dynamic, Dynamic>();         // ��s�����v���ɪ��e�ȥ�(��l PWeight)
	MatrixXd* DBWeight                           = new MatrixXd(B->cast<double>());                // ������s�᪺ B �v��(int    ���A)
	MatrixXi* UpdateIBWeight                     = nullptr;                                        // ������s�᪺ B �v��(double ���A)
	MatrixXd* UpdateDBWeight                     = nullptr;                // �N�쥻 int ���A �V�m��Ƽ���(ttran) ������s�v�� �ഫ�� double ���A
	MatrixXd* DyOneHotLabel                      = new MatrixXd(IyOneHotLabel->cast<double>());
	
	//G-STEP
	if (Gmap_loss == "L2")
	{
		if (itr == 0)                             // ���ͪ�l W ������s�v�� 
		{
			auto Wg                = WWeight_client(DBWeight, IyOneHotLabel, Gmap_Lambda, SwitchMethod);
			WWeight                = get<0>(Wg);
		}
		else                                      // ���ͲĤG���H�� W ��������s�v�� 
		{
			auto Wg                = WWeightRRC_client(DBWeight, IyOneHotLabel, Gmap_Lambda, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath);
			WWeight                = get<0>(Wg);
			start_WaitAggreWeight  = get<1>(Wg);  // �������� Server �Ǧ^�E�X�v���}�l�ɶ��W
			end_WaitAggreWeight    = get<2>(Wg);  // �������� Server �Ǧ^�E�X�v�������ɶ��W
		}
	}
	//F-STEP
	if (itr == 0)                                 // ���ͪ�l P ������s�����v�� 
	{
		auto WF                    = PWeight_client(X, B, Fmap_Lambda, LabelCount);
		PWeight                    = get<0>(WF);
		WF0                        = get<0>(WF);
	}
	else                                          // ���ͲĤG���H�� W ��������s�v��
	{
		auto WF                    = PWeightRRC_client(X, B, nu, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath, LabelCount);
		PWeight                    = get<0>(WF);
		WF0                        = get<0>(WF);
	}

	while (localitr < LocalIteration)
	{
		localitr++;
		if ((SwitchMethod == "PROX_SDH") || (SwitchMethod == "AVG_SDH"))
		{
			//B-STEP
			MatrixXd* XF   = new MatrixXd((*X) * (*PWeight));
			MatrixXd* Q    = new MatrixXd(MatrixXd::Zero(B->rows(), B->cols()));
			//Load SERVER INIT B
			if (itr == 0)  // ���ͲĤ@�� Q ������s�����v��
			{
				*Q   = nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
			}
			else           // ���ͲĤG���H�� Q ������s�����v��
			{
                if (SwitchMethod == "AVG_SDH")
                {
                    *Q   = nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
                }
                else if(SwitchMethod == "PROX_SDH")
                {
					//�|�Ӫ� ��Bpre�����~
                    //*Q   = (mu / 2) * (*Bpre) + nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
				    //�ץ��� 20230726
				    *Q   =                      nu * (*XF) + (*DyOneHotLabel) * (*WWeight).transpose();
                }
            }
			for (int time = 0; time < 10; time++)
			{
				Bpre = new MatrixXd(B->cast<double>());
				for (int k = 0; k < B->cols(); k++)
				{
					MatrixXd* Zk  = new MatrixXd(B->cast<double>());
					RemoveColumn(Zk, k);
					MatrixXd* Wkk = new MatrixXd((*WWeight).row(k));
					MatrixXd* Wk  = new MatrixXd(*WWeight);;
					RemoveRow(Wk, k);
					//LinForm N-by-1
					MatrixXd* LinForm = new MatrixXd((*Q).col(k) - (*Zk) * (*Wk) * (*Wkk).transpose());
					if (quadFormWght > 0)
					{
						quadFormWght++;
					}
					else
					{
						B->col(k) = (linFormWght * (*LinForm)).array().sign().matrix().cast<int>();  // ���ͧ�����s�᪺ B
					}
					if (quadFormWght > 0)
					{
						//cout << 'b' << endl;
					}
					// �����ʺA���t�O����
					delete Zk;
					delete Wkk;
					delete Wk;
					delete LinForm;
				}
				UpdateDBWeight = new MatrixXd(B->cast<double>());
				UpdateIBWeight = B;
				if (((*UpdateDBWeight) - (*Bpre)).squaredNorm() < 1e-6 * (*Bpre).squaredNorm())
				{
					break;
				}
				// �����ʺA���t�O����
				delete Bpre;
			}
			// �����ʺA���t�O����
			delete XF;
			delete Q;
		}
		else if ((SwitchMethod == "PROX_FSDH") || (SwitchMethod == "AVG_FSDH"))
		{
			//B-STEP
			MatrixXd* FSDH_XF = new MatrixXd((*X) * (*PWeight));
			if (itr == 0)
			{
				UpdateIBWeight = new MatrixXi(((*DyOneHotLabel) * (*WWeight) + nu * (*FSDH_XF)).array().sign().matrix().cast<int>());  // ���ͧ�����s�᪺ B
			}
			else
			{
                if (SwitchMethod == "AVG_FSDH")
                {
                    UpdateIBWeight = new MatrixXi(((*DyOneHotLabel) * (*WWeight) + nu * (*FSDH_XF)).array().sign().matrix().cast<int>());  // ���ͧ�����s�᪺ B
                }
                else if(SwitchMethod == "PROX_FSDH")
                {
					//�|�Ӫ� ��Bpre�����~
				    //UpdateIBWeight = new MatrixXi(((mu / 2) * (*Bpre) + nu * (*FSDH_XF) + (*DyOneHotLabel) * (*WWeight)).array().sign().matrix().cast<int>());  // ���ͧ�����s�᪺ B
				    //�ץ��� 20230726
				    UpdateIBWeight = new MatrixXi((                     nu * (*FSDH_XF) + (*DyOneHotLabel) * (*WWeight)).array().sign().matrix().cast<int>());  // ���ͧ�����s�᪺ B
                }
			}
			UpdateDBWeight = new MatrixXd(UpdateIBWeight->cast<double>());
		}
		//G-STEP
		if (Gmap_loss == "L2")
		{
			if (itr == 0)
			{
				auto Wg = WWeight_client(UpdateDBWeight, IyOneHotLabel, Gmap_Lambda, SwitchMethod);
				WWeight = get<0>(Wg);
			}
			else
			{
				auto Wg = WWeightRRC_client(UpdateDBWeight, IyOneHotLabel, Gmap_Lambda, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath);
				WWeight = get<0>(Wg);
			}

		}
		//F-STEP
		if (itr == 0)
		{
			auto WF = PWeight_client(X, UpdateIBWeight, Fmap_Lambda, LabelCount);
			PWeight = get<0>(WF);
		}
		else
		{
			auto WF = PWeightRRC_client(X, UpdateIBWeight, nu, mu, str_EXP, str_ITR, SwitchMethod, LoadDataPath, LabelCount);
			PWeight = get<0>(WF);
		}
		//��s�᪺B�A��ȵ�UpdateDBWeight
		double bias = ((*UpdateDBWeight) - (*X) * (*PWeight)).squaredNorm();
		if (debug)
		{
			//cout << "bias = " << bias << endl;
		}

		if (bias < tol * (*UpdateDBWeight).squaredNorm())
		{
			break;
		}

		if ((*PWeight - *WF0).squaredNorm() < tol * (*WF0).squaredNorm())
		{
			break;
		}

	}
	// LoadDataPath = "..\\weightPoolPath\\SERVER2NANO_"
	string cutDataPath_1 = LoadDataPath.substr(0, LoadDataPath.length() - 12);
	string cutDataPath_2 = LoadDataPath.substr(LoadDataPath.length() - 5 , 4);
	string renewDataPath = cutDataPath_1 + cutDataPath_2 + "2SERVER_";
	//�x�s��s���v��
	string CLIENT_to_SEVER_WWeight   = renewDataPath + str_EXP + "_" + str_ITR + INIT_CLIENT_WWeightMtxTxt;
	string CLIENT_to_SEVER_PWeight   = renewDataPath + str_EXP + "_" + str_ITR + INIT_CLIENT_PWeightMtxTxt;
    string CLIENT_to_SEVER_LabelList = renewDataPath + str_EXP + INIT_CLIENT_LabelListMtxTxt;
	// �Ǧ^ Server ��
	if ((SwitchMethod == "PROX_SDH") || (SwitchMethod == "AVG_SDH"))
	{
		//MatrixXd* SDH_WWeight = new MatrixXd(std::get<0>(WWeight));
		MatrixXd* SDH_WWeight = new MatrixXd((*WWeight).transpose());
		double2txt_client(SDH_WWeight, CLIENT_to_SEVER_WWeight);
	}
	else if ((SwitchMethod == "PROX_FSDH") || (SwitchMethod == "AVG_FSDH"))
	{
		double2txt_client(WWeight, CLIENT_to_SEVER_WWeight);

	}

	//double2txt_client(WWeight        , CLIENT_to_SEVER_WWeight);
	double2txt_client(PWeight        , CLIENT_to_SEVER_PWeight);
    if (itr == 0)
    {
        double2txt_client(&RenewLabelList, CLIENT_to_SEVER_LabelList);
    }

	


	return std::make_tuple(WWeight, PWeight, B, start_WaitAggreWeight, end_WaitAggreWeight);

}
