// WNruntimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinNginx.h"
#include "WNruntimeDlg.h"
#include "afxdialogex.h"
#include "WNGlobal.h"

// WNruntimeDlg 对话框
WNGlobal rwng;
IMPLEMENT_DYNAMIC(WNruntimeDlg, CDialogEx)

WNruntimeDlg::WNruntimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(WNruntimeDlg::IDD, pParent)
{

}

WNruntimeDlg::~WNruntimeDlg()
{
}

void WNruntimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_RUNTIME, m_list);
}


BEGIN_MESSAGE_MAP(WNruntimeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_INSTALL, &WNruntimeDlg::OnBtnStartInstall)
END_MESSAGE_MAP()


// WNruntimeDlg 消息处理程序
BOOL WNruntimeDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	Init();
	return TRUE;
}
void WNruntimeDlg::Init(){
	m_list.AddString(_T("VC9 RunTime x86"));
	m_list.AddString(_T("VC11 RunTime x86"));
	m_list.AddString(_T("VC14 RunTime x86"));
	m_list.AddString(_T("VC15 RunTime x86"));
	m_list.SetCurSel(0);
}

void WNruntimeDlg::OnBtnStartInstall()
{
	int nIndex = m_list.GetCurSel();
	//CString strCBText;
	//m_list.GetLBText(nIndex, strCBText);
	//GetWindowText(strCBText);
	//AfxMessageBox(rwng.Int2CString(nIndex));
	if (nIndex== 0)
	{
		CString dir =rwng.GetAppPath() + _T("\\redistributable\\vcredist_x86_9.exe");
		rwng.ShellRun(dir, _T(""), SW_SHOW);
	}
	if (nIndex == 1)
	{
		CString dir = rwng.GetAppPath() + _T("\\redistributable\\vcredist_x86_11.exe");
		rwng.ShellRun(dir, _T(""), SW_SHOW);
	}
	if (nIndex == 2)
	{
		CString dir = rwng.GetAppPath() + _T("\\redistributable\\vcredist_x86_14.exe");
		rwng.ShellRun(dir, _T(""), SW_SHOW);
	}
	if (nIndex == 3)
	{
		CString dir = rwng.GetAppPath() + _T("\\redistributable\\vcredist_x86_15.exe");
		rwng.ShellRun(dir, _T(""), SW_SHOW);
	}
}
