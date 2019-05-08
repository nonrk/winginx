// WNbackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinNginx.h"
#include "WNbackDlg.h"
#include "afxdialogex.h"
#include "WNGlobal.h"

WNGlobal bwng;

IMPLEMENT_DYNAMIC(CWNbackDlg, CDialogEx)

CWNbackDlg::CWNbackDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWNbackDlg::IDD, pParent)
{

}

CWNbackDlg::~CWNbackDlg()
{
}

void CWNbackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DIR, m_dir);
	DDX_Control(pDX, IDC_BTN_RESTORE, m_btn_restore);
	DDX_Control(pDX, IDC_BTN_START_BACK, m_btn_start);
	DDX_Control(pDX, IDC_CK_CONFIG, m_ck_config);
	DDX_Control(pDX, IDC_CK_DATA, m_ck_data);
	DDX_Control(pDX, IDC_CK_SITE, m_ck_site);
	DDX_Control(pDX, IDC_CK_ALL, m_ck_all);
}


BEGIN_MESSAGE_MAP(CWNbackDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_BACK, &CWNbackDlg::OnBtnStartBack)
	ON_BN_CLICKED(IDC_BTN_RESTORE, &CWNbackDlg::OnBtnRestore)
	ON_BN_CLICKED(IDC_BTN_DIR, &CWNbackDlg::OnBtnDir)
	ON_BN_CLICKED(IDC_CK_ALL, &CWNbackDlg::OnCkAll)
	ON_BN_CLICKED(IDC_CK_CONFIG, &CWNbackDlg::OnCkConfig)
	ON_BN_CLICKED(IDC_CK_DATA, &CWNbackDlg::OnCkData)
	ON_BN_CLICKED(IDC_CK_SITE, &CWNbackDlg::OnCkSite)
END_MESSAGE_MAP()


// CWNbackDlg 消息处理程序
BOOL CWNbackDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	Init();
	return TRUE;
}
void CWNbackDlg::Init(){
	m_dir.SetWindowText(bwng.GetAppPath() + _T("\\application"));
	CString dirPath;
	m_dir.GetWindowText(dirPath);
	if (dirPath.GetLength()==0)
	{
		m_btn_start.EnableWindow(FALSE);
		m_btn_restore.EnableWindow(FALSE);
	}
}
void CWNbackDlg::OnBtnStartBack()
{
	CString targetDir;
	m_dir.GetWindowText(targetDir);
	CFileFind m_sFileFind;
	bwng.makeFolder(targetDir + _T("\\winginx_back"));
	if (m_sFileFind.FindFile(targetDir + _T("\\winginx_back")))
	{
		UINT i;
		i = MessageBox(_T("目标文件已存在，是否覆盖？"), _T("温馨提示"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDNO)
		{
			return;
		}
		else
		{
			bwng.deleteDirectory(targetDir + _T("\\winginx_back"));
			bwng.makeFolder(targetDir + _T("\\winginx_back"));
			if (m_ck_config.GetCheck())
			{
				//备份配置文件
				bwng.copyFiles(bwng.GetAppPath() + _T("\\application\\mysql\\my"), targetDir + _T("\\winginx_back\\my"), _T("ini"));
				bwng.copyFiles(bwng.GetAppPath() + _T("\\application\\php\\php"), targetDir + _T("\\winginx_back\\php"), _T("ini"));
			}
			if (m_ck_data.GetCheck())
			{
				bwng.copyDirectory(bwng.GetAppPath() + _T("\\application\\mysql\\data"), targetDir + _T("\\winginx_back\\data"), TRUE);
			}
			if (m_ck_site.GetCheck())
			{
				bwng.copyDirectory(bwng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost"), targetDir + _T("\\winginx_back\\vhost"), TRUE);
			}
			AfxMessageBox(_T("备份完成！"));
			CDialog::EndDialog(0);
		}
	}
	
}


void CWNbackDlg::OnBtnRestore()
{
	CString targetDir;
	m_dir.GetWindowText(targetDir);
	CFileFind m_sFileFind;

	if (m_sFileFind.FindFile(targetDir + _T("\\winginx_back")))
	{
		UINT i;
		i = MessageBox(_T("恢复文件将覆盖现有文件？"), _T("温馨提示"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDNO)
		{
			return;
		}
		else
		{
			if (m_ck_config.GetCheck())
			{
				//还原配置文件
				if (bwng.IsFileExist(targetDir + _T("\\winginx_back\\my.ini")))
				{
					DeleteFile(bwng.GetAppPath() + _T("\\application\\mysql\\my.ini"));
					bwng.copyFiles(targetDir + _T("\\winginx_back\\my"), bwng.GetAppPath() + _T("\\application\\mysql\\my"), _T("ini"));
				}
				if (bwng.IsFileExist(targetDir + _T("\\winginx_back\\my.ini"))){
					DeleteFile(bwng.GetAppPath() + _T("\\application\\php\\php.ini"));
					bwng.copyFiles(targetDir + _T("\\winginx_back\\php"), bwng.GetAppPath() + _T("\\application\\php\\php"), _T("ini"));
				}
				
			}
			if (m_ck_data.GetCheck())
			{
				if (m_sFileFind.FindFile(targetDir + _T("\\winginx_back\\data")))
				{
					bwng.deleteDirectory(bwng.GetAppPath() + _T("\\application\\mysql\\data"));
					bwng.copyDirectory(targetDir + _T("\\winginx_back\\data"), bwng.GetAppPath() + _T("\\application\\mysql\\data"), TRUE);
				}
				
			}
			if (m_ck_site.GetCheck())
			{
				if (m_sFileFind.FindFile(targetDir + _T("\\winginx_back\\vhost"))){
					bwng.deleteDirectory(bwng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost"));
					bwng.copyDirectory(targetDir + _T("\\winginx_back\\vhost"), bwng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost"), TRUE);
				}
				
			}
			AfxMessageBox(_T("备份完成！"));
			CDialog::EndDialog(0);
		}
	}
	else
	{
		AfxMessageBox(_T("备份目录无备份文件！"));
	}

}


void CWNbackDlg::OnBtnDir()
{
	CString dirPath;
	dirPath = bwng.GetDirectory();
	m_dir.SetWindowText(dirPath);
	dirPath.ReleaseBuffer();
	if (dirPath.GetLength()!=0)
	{
		m_btn_start.EnableWindow(TRUE);
		m_btn_restore.EnableWindow(TRUE);
	}
	else
	{
		AfxMessageBox(_T("备份目录不存在"));
	}
	
}



void CWNbackDlg::OnCkAll()
{
	
	if (m_ck_all.GetCheck())
	{

		m_ck_config.SetCheck(1);
		m_ck_data.SetCheck(1);
		m_ck_site.SetCheck(1);
	}
	else
	{
		m_ck_config.SetCheck(0);
		m_ck_data.SetCheck(0);
		m_ck_site.SetCheck(0);
	}
}

void CWNbackDlg::checkCheckBox()
{
	int a, b, c;
	a = m_ck_config.GetCheck();
	b = m_ck_data.GetCheck();
	c = m_ck_site.GetCheck();
	if (a + b + c >= 3){
		m_ck_all.SetCheck(1);
	}
	else
	{
		m_ck_all.SetCheck(0);
	}
}

void CWNbackDlg::OnCkConfig()
{
	checkCheckBox();
}


void CWNbackDlg::OnCkData()
{
	checkCheckBox();
}


void CWNbackDlg::OnCkSite()
{
	checkCheckBox();
}
