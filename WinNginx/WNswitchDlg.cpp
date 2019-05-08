// WNswitchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinNginx.h"
#include "WNswitchDlg.h"
#include "afxdialogex.h"
#include "WNGlobal.h"

#define PHP_VSRSION_FIVETWO 52
#define PHP_VSRSION_FIVESEX 56
#define PHP_VSRSION_SEVEN 70
#define PHP_VSRSION_SEVENONE 71
#define PHP_VSRSION_SEVENTWO 72
#define PHP_VSRSION_SEVENTHREE 73


WNGlobal swng;
IMPLEMENT_DYNAMIC(CWNswitchDlg, CDialogEx)

CWNswitchDlg::CWNswitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWNswitchDlg::IDD, pParent)
{

}

CWNswitchDlg::~CWNswitchDlg()
{
}

void CWNswitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATE, m_status);
	DDX_Control(pDX, IDC_SW_VERSION, m_version);
	DDX_Control(pDX, IDC_PROGRESS_SWITCH, m_progress);
}


BEGIN_MESSAGE_MAP(CWNswitchDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CWNswitchDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	Init();
	return TRUE;
}
DWORD CALLBACK lpProgressRoutine(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize, LARGE_INTEGER StreamBytesTransferred, DWORD dwStreamNumber, DWORD dwCallbackReason, HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData) {

	CWNswitchDlg *cl_this = (CWNswitchDlg *)lpData;
	static int nRecord = 0;
	nRecord++;
	if (dwCallbackReason == CALLBACK_STREAM_SWITCH) // �����һ�ε���ʱ��ΪTRUE, ֮����FALSE. 
	{
		cl_this->m_progress.SetRange(0, TotalFileSize.LowPart);
	}
	CString pStr;
	pStr.Format(_T("�ص�������%d    �Ѵ��䣺%08X:%08X    �ļ���С��%08X:%08X "),
		nRecord,
		TotalBytesTransferred.HighPart,
		TotalBytesTransferred.LowPart,
		TotalFileSize.HighPart,
		TotalFileSize.LowPart);
	cl_this->m_status.SetWindowText(pStr);
	cl_this->m_progress.SetPos(TotalBytesTransferred.LowPart);
	return PROGRESS_CONTINUE;
}

DWORD CALLBACK ThreadProc(LPVOID lpParameter) {
	CWNswitchDlg * cl_this = (CWNswitchDlg *)lpParameter;
	CString str1, str2;
	AfxMessageBox(swng.Int2CString(CopyFileEx(str1, str2, (LPPROGRESS_ROUTINE)lpProgressRoutine, cl_this, FALSE, COPY_FILE_RESTARTABLE)));
	if (!CopyFileEx(cl_this->m_strPath, cl_this->m_desPath, (LPPROGRESS_ROUTINE)lpProgressRoutine, cl_this, FALSE, COPY_FILE_RESTARTABLE))
	{
		//AfxMessageBox(_T("�л�phpʧ��"));
		//cl_this->OnDestroy();
	}
	return 0;
}
void CWNswitchDlg::Init(){
	switch (version)
	{
	case PHP_VSRSION_FIVETWO:
		m_version.SetWindowText(_T("php 5.6"));
		m_status.SetWindowText(_T("�汾�л���..."));
		CopyFile(swng.GetAppPath() + _T("\\application\\php\\php.ini"), swng.GetAppPath() + _T("\\application\\php_ext\\php_back.ini"),TRUE);
		m_strPath = swng.GetAppPath()+_T("\\application\\php_ext\\php_5.6");
		m_desPath = swng.GetAppPath() + _T("\\application\\php1");
		if ((m_strPath.IsEmpty() && m_desPath.IsEmpty()))
		{
			AfxMessageBox(_T("·������Ϊ�գ�"));
			return;
		}

		else
		{
			//�����߳�
			HANDLE thread = CreateThread(NULL, 0, ThreadProc, this, NULL, NULL);
			CloseHandle(thread);
		}
		break;
	default:
		break;
	}
}

void CWNswitchDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

