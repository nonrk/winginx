// WNaddwebDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinNginx.h"
#include "WNaddwebDlg.h"
#include "afxdialogex.h"
#include "WNGlobal.h"
#include "WinNginxDlg.h"
WNGlobal awng;
CWinNginxDlg *awn;
// CWNaddwebDlg 对话框
IMPLEMENT_DYNAMIC(CWNaddwebDlg, CDialogEx)

CWNaddwebDlg::CWNaddwebDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWNaddwebDlg::IDD, pParent)
{

}

CWNaddwebDlg::~CWNaddwebDlg()
{
}

void CWNaddwebDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_EDIT_DOMAIN, m_domain);
	DDX_Control(pDX, IDC_ADD_EDIT_ROOT, m_root);
	DDX_Control(pDX, IDC_ADD_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_ADD_CK_HTTPS, m_check_https);
	DDX_Control(pDX, IDC_ADD_BTN_SAVE, m_btn_save);
	DDX_Control(pDX, IDC_ADD_EDIT_NAME, m_name);
}


BEGIN_MESSAGE_MAP(CWNaddwebDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_BTN_FILE, &CWNaddwebDlg::OnBtnOpenDir)
	ON_BN_CLICKED(IDC_ADD_BTN_SAVE, &CWNaddwebDlg::OnBtnSave)
END_MESSAGE_MAP()


// CWNaddwebDlg 消息处理程序
BOOL CWNaddwebDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	return TRUE;
}
void CWNaddwebDlg::Init(){
	//初始化默认目录
	m_root.SetWindowText(awng.GetAppPath() + _T("\\wwwroot"));
	m_port.SetWindowText(_T("80"));
}
void CWNaddwebDlg::AddWebSite(CString root, CString name, CString port, CString domain,int https=0)
{
	CString tempFile;
	CString str;
	if (https)
	{
		tempFile = _T("vhost_https_template.conf");
	}
	else
	{
		tempFile = _T("vhost_http_template.conf");
	}
	CStdioFile myFile, File;
	CString strText;
	if (myFile.Open(awng.GetAppPath() + _T("\\application\\nginx\\conf\\" + tempFile), CFile::modeRead))
	{
		//读取
		while (myFile.ReadString(str))
		{
			strText += str+_T("\r\n");
		}
		myFile.Close();
	}
	else
	{
		AfxMessageBox(_T("模板文件不存在"));
	}
	
	//替换关键字
	strText.Replace(_T("SERVER_DOMAIN"), domain);
	strText.Replace(_T("SERVER_PORT"), port);
	strText.Replace(_T("SERVER_ROOT"), root);
	//创建
	//File.SeekToEnd();
	if (!awng.IsFileExist(awng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")))){
		File.Open(awng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")), CFile::modeCreate | CFile::modeReadWrite);
		File.WriteString(strText);
		if (awng.IsFileExist(awng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")))){
			awng.setConfig(_T("网站配置列表"), name, name + _T("|") + domain + _T("|") + port + _T("|") + root, awng.GetAppPath() + _T("\\\\application\\webList.ini"));
			if (AfxMessageBox(_T("网站创建成功，是否重启服务器？"), MB_OKCANCEL) == IDOK){
				awn->OnBtnRestart();
			}
		}
		strText.ReleaseBuffer();
		File.Close();
		CDialog::EndDialog(0);
	}
	else
	{
		AfxMessageBox(_T("该网站已经存在"));
	}
}
void CWNaddwebDlg::OnBtnOpenDir()
{
	CString nPath;
	nPath=awng.GetDirectory();
	m_root.SetWindowText(nPath);
}

void CWNaddwebDlg::OnBtnSave()
{
	int https;
	https = m_check_https.GetCheck();
	CString domain;
	CString name;
	CString root;
	CString port;
	m_domain.GetWindowText(domain);
	m_name.GetWindowText(name);
	m_root.GetWindowText(root);
	m_port.GetWindowText(port);
	if (https)
	{
		if (domain.GetLength()>0 && root.GetLength()>0 && name.GetLength()>0 && port.GetLength()>0)
		{
			AddWebSite(root, name, port, domain, https);
		}
		else
		{
			AfxMessageBox(_T("参数不完整"));
		}
	}
	else
	{
		if (domain.GetLength() > 0 && root.GetLength() > 0 && name.GetLength() > 0 && port.GetLength() > 0)
		{
			AddWebSite(root, name, port, domain, https);
		}
		else
		{
			AfxMessageBox(_T("参数不完整"));
		}
	}
}

