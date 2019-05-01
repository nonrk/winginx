// WnweblistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinNginx.h"
#include "WnweblistDlg.h"
#include "afxdialogex.h"
#include "WNGlobal.h"

// CWnweblistDlg 对话框
WNGlobal wwng;
IMPLEMENT_DYNAMIC(CWnweblistDlg, CDialogEx)

CWnweblistDlg::CWnweblistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWnweblistDlg::IDD, pParent)
{

}

CWnweblistDlg::~CWnweblistDlg()
{
}

void CWnweblistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CWnweblistDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WEB, &CWnweblistDlg::OnNMDblclkWeb)
END_MESSAGE_MAP()
BOOL CWnweblistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	return TRUE;
}

// CWnweblistDlg 消息处理程序
void CWnweblistDlg::Init()
{
	//初始化日志
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.InsertColumn(0, _T("网站名称"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("网站域名"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(2, _T("网站端口"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(3, _T("网站根目录"), LVCFMT_LEFT, 150);
	vector<CString>list = getWebList();
	vector<CString>item;
	if (list.size()>0)
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			item = wwng.SplitCString(list[i], _T("|"));
			m_list.InsertItem(i, item[0]);
			m_list.SetItemText(i, 1, item[1]);
			m_list.SetItemText(i, 2, item[2]);
			m_list.SetItemText(i, 3, item[3]);
		}
	}
	//wng.writeLog(&m_list, _T("欢迎使用WinNginx"));
}
vector<CString> CWnweblistDlg::getWebList(){
	//vector<CString> config;
	vector<CString> result;
	TCHAR szKey[1024] = { 0 };
	CString strKey = _T("");
	CString strKeyName = _T("");
	CString strKeyValue = _T("");
	TCHAR szBuffer[65536] = { 0 };
	CString strSectionName = _T("");
	//获得INI文件指定段的全部键名和键值
	int nBufferSize = GetPrivateProfileSection(_T("网站配置列表"), szBuffer, 65536, wwng.GetAppPath() + _T("\\application\\webList.ini"));
	int nItem = 0;
	for (int n = 0, i = 0; n < nBufferSize; n++)
	{
		if (szBuffer[n] == 0)
		{
			szKey[i] = 0;
			strKey = szKey;
			strKeyName = strKey.Left(strKey.Find('='));
			strKeyValue = strKey.Mid(strKey.Find('=') + 1);
			//config.push_back(strKeyName);
			result.push_back(strKeyValue);
			i = 0;
			nItem++;
		}
		else
		{
			szKey[i] = szBuffer[n];
			i++;
		}
	}
	return result;
}
void CWnweblistDlg::OnNMDblclkWeb(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION p = m_list.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE("没有行被选中!");
	}
	int index = m_list.GetNextSelectedItem(p);

	CString name = m_list.GetItemText(index, 0);
	CString domain = m_list.GetItemText(index, 1);
	CString port = m_list.GetItemText(index, 3);
	CString root = m_list.GetItemText(index, 4);

}
