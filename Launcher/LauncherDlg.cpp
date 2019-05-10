
// LauncherDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include <string>
#include "curl/curl.h"
#include <urlmon.h>
#include "BindCBHttpCallback.h"
#include "WinInet.h"
#pragma comment(lib,"wininet.lib")
#include <process.h>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "libcurl.lib") 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
enum
{
	PRO_POROGRESS = 101,
	PRO_POROGRESS_TOTAL = 102,
	PRO_TEXT = 103,
	PRO_TEXT_TOTAL = 104,
	PRO_TEXT_FILE = 105

};
CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_PRO_INFO, m_msg);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_process_total);
	DDX_Control(pDX, IDC_PRO_TOTAL, m_msg_total);
	DDX_Control(pDX, IDC_PRO_MSG, m_total);
	DDX_Control(pDX, IDC_START, m_start);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CLauncherDlg::OnBnStart)
END_MESSAGE_MAP()


BOOL CLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// TODO:  在此添加额外的初始化代码
	return TRUE; 
}


void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CLauncherDlg::GetAppPath()
{
	TCHAR tszBuf[MAX_PATH] = { '\0' };
	GetModuleFileName(NULL, tszBuf, MAX_PATH);
	CString strDir, tmpDir;
	tmpDir = tszBuf;
	strDir = tmpDir.Left(tmpDir.ReverseFind('\\'));
	return strDir;
}
//CURL
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	/* FILE *fptr = (FILE*)userp;
	fwrite(buffer, size, nmemb, fptr);

	return size * nmemb;;*/
	std::string* str = dynamic_cast<std::string*>((std::string *)userp);
	if (NULL == str || NULL == buffer)
	{
		return -1;
	}

	char* pData = (char*)buffer;
	if (pData)
	{
		str->append(pData, size * nmemb);
	}
	return nmemb;
}

CString CLauncherDlg::httpGet(char* Url)
{
	CURL *curl;
	CURLcode res;
	std::string fptr;
	struct curl_slist *http_header = NULL;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, Url);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _T(""));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //注意回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&fptr); //传的引用
	curl_easy_setopt(curl, CURLOPT_POST, 0);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	res = curl_easy_perform(curl);
	CString str;
	str = fptr.c_str();
	if (res != CURLE_OK)
	{
		return FALSE;
	}
	curl_easy_cleanup(curl);
	return str;
}
CString CLauncherDlg::httpPost(char* Url, char* params)
{
	CURL *curl;
	CURLcode res;
	std::string fptr;
	struct curl_slist *http_header = NULL;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, Url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //注意回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&fptr); //传的引用
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	res = curl_easy_perform(curl);
	CString str;
	str = fptr.c_str();
	if (res != CURLE_OK)
	{
		return FALSE;
	}
	curl_easy_cleanup(curl);
	return str;
}
void CLauncherDlg::setMsg(int type, int data = 0, CString strData = NULL, int data_type=0){
	switch (type)
	{
	case PRO_TEXT:
		if (data_type)
		{
			CString szText;
			szText.Format(data);
			m_msg.SetWindowText(szText);
		}
		else
		{
			m_msg.SetWindowText(strData);
		}
		break;
	case PRO_TEXT_TOTAL:
		if (data_type)
		{
			CString szText;
			szText.Format(data);
			m_msg_total.SetWindowText(szText);
		}
		else
		{
			m_msg_total.SetWindowText(strData);
		}
		break;
	case PRO_TEXT_FILE:
		if (data_type)
		{
			CString szText;
			szText.Format(data);
			m_total.SetWindowText(szText);
		}
		else
		{
			m_total.SetWindowText(strData);
		}
		break;

	default:
		break;
	}
}
int CLauncherDlg::getFileLength(CString filePath){
	CFile cfile;
	if (cfile.Open(filePath, CFile::modeRead))
	{
		int size = cfile.GetLength();
		return size;
	}
	else
	{
		return FALSE;
	}
}
void CLauncherDlg::setProgress(int type, int data ){
	switch (type)
	{
	case PRO_POROGRESS:
		m_progress.SetRange32(0, 100);
		m_progress.SetPos((int)(data));
		break;
	case PRO_POROGRESS_TOTAL:
		m_process_total.SetRange32(0, 100);
		m_process_total.SetPos((int)(data));
		break;
	default:
		break;
	}

}
unsigned int __stdcall CLauncherDlg::DownLoadProcess(void* pParam)
{
	CLauncherDlg* dlg = (CLauncherDlg*)pParam;
	CBindCBHttpCallback callBack;
	callBack.m_pDlg = dlg;
	CString szURL;
	CString szPath;
	CLauncherDlg cDlg;

	

	

	szURL.Format(_T("http://192.168.1.99/19-11-12-22-17-28.updata?%d"), GetTickCount());
	szPath.Format(_T("%s\\download\\19-11-12-22-17-28.updata"), cDlg.GetAppPath());

	if (S_OK == URLDownloadToFile(NULL, szURL, szPath, 0, &callBack))
	{
		
	}
	return TRUE;
}
void CLauncherDlg::setStatus(){
	CString szPath;
	setMsg(103, 0, _T("下载完成"), 0);
	CString szMsg;
	szMsg.Format(_T("%d%%"), 100);
	setMsg(104, 0, szMsg, 0);
	CString f_size;
	szPath.Format(_T("%s\\download\\19-11-12-22-17-28.updata"), GetAppPath());
	f_size.Format(_T("%dkb"), getFileLength(szPath) / 1024);
	setMsg(105, 0, f_size, 0);
	setProgress(102, 100);
	m_start.EnableWindow(TRUE);
}
void CLauncherDlg::OnBnStart()
{
	
	m_start.EnableWindow(FALSE);
	setMsg(105, 0, _T("1/1"), 0);
	CString szText;
	szText.Format(_T("%d%%"), 0);
	setMsg(104, 0, szText,0);
	setProgress(102, 50);
	unsigned int nThread = 0;
	HANDLE pThreadDownload = (HANDLE)_beginthreadex(NULL, 0, &DownLoadProcess, this, 0, &nThread);
	if (pThreadDownload)
	{
		::CloseHandle(pThreadDownload);
		pThreadDownload = NULL;
	
		
	}
}
