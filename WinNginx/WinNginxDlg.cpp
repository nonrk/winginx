
#include "stdafx.h"
#include "afxdialogex.h"
#include "windows.h"
#include "winbase.h"
#include <shellapi.h>
#include <atlconv.h>
#include <msi.h>
#include "WinNginx.h"
#include "WinNginxDlg.h"
#include "WNaddwebDlg.h"
#include "WNGlobal.h"
#include "WnweblistDlg.h"
#include "WNbackDlg.h"
#include "WNruntimeDlg.h"
#pragma comment(lib,"Msi.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOWTASK (WM_USER +1)
#define IDR_SHOW 11
#define IDR_SYS_START 12
#define IDR_SYS_STOP 13
#define IDR_SYS_RESTART 14
#define IDR_SYS_EXIT 15

#define PHP_VSRSION_FIVETWO 52
#define PHP_VSRSION_FIVESEX 56
#define PHP_VSRSION_SEVEN 70
#define PHP_VSRSION_SEVENONE 71
#define PHP_VSRSION_SEVENTWO 72
#define PHP_VSRSION_SEVENTHREE 73

WNGlobal wng;
vector <int> serverState = {0,0,0};
long start_time;
int php_version;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
public:

};
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

CWinNginxDlg::CWinNginxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinNginxDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinNginxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_SYSTEM, m_info_system);
	DDX_Control(pDX, IDC_INFO_PHP, m_info_php);
	DDX_Control(pDX, IDC_INFO_NGINX, m_info_nginx);
	DDX_Control(pDX, IDC_INFO_MYSQL, m_info_mysql);
	DDX_Control(pDX, IDC_INFO_VERSION, m_version);
	DDX_Control(pDX, IDC_STATE_PHP, m_state_php);
	DDX_Control(pDX, IDC_STATE_NGINX, m_state_nginx);
	DDX_Control(pDX, IDC_STATE_MYSQL, m_state_mysql);
	DDX_Control(pDX, IDC_STATE_SERVER, m_state_server);
	DDX_Control(pDX, IDC_LIST_LOG, m_log);
	DDX_Control(pDX, IDC_BTN_START, m_btn_start);
	DDX_Control(pDX, IDC_BTN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BTN_RESTART, m_btn_restart);
	DDX_Control(pDX, IDC_BTN_ADD, m_btn_add);
	DDX_Control(pDX, IDC_BTN_DOMAIN, m_btn_web);
	DDX_Control(pDX, IDC_BTN_RUNTIME, m_btn_runtime);
	DDX_Control(pDX, IDC_BTN_SERVERBACK, m_btn_server_back);
}

BEGIN_MESSAGE_MAP(CWinNginxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START, &CWinNginxDlg::OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CWinNginxDlg::OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_RESTART, &CWinNginxDlg::OnBtnRestart)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_COMMAND(ID_MENU_ABOUT_COMPRIGHT, &CWinNginxDlg::OnMenuAboutCompright)
	ON_BN_CLICKED(IDC_BTN_DOMAIN, &CWinNginxDlg::OnBtnWebManage)
	ON_BN_CLICKED(IDC_BTN_ADD, &CWinNginxDlg::OndBtnAddWeb)
	ON_BN_CLICKED(IDC_BTN_RUNTIME, &CWinNginxDlg::OnBtnCheckRuntime)
	ON_BN_CLICKED(IDC_BTN_SERVERBACK, &CWinNginxDlg::OnBtnServerback)
	ON_COMMAND(ID_PHP_FIVETWO, &CWinNginxDlg::OnMenuFiveTwo)
	ON_COMMAND(ID_PHP_FIVESIX, &CWinNginxDlg::OnMenuFiveSix)
	ON_COMMAND(ID_PHP_SEVEN, &CWinNginxDlg::OnMenuSeven)
	ON_COMMAND(ID_PHP_SEVEN_ONE, &CWinNginxDlg::OnMenuSevenOne)
	ON_COMMAND(ID_PHP_SEVENTWO, &CWinNginxDlg::OnMenuSevenTwo)
	ON_COMMAND(ID_PHP_SEVENTHREE, &CWinNginxDlg::OnMenuSevenThree)
	ON_COMMAND(ID_MENU_RUNTIME, &CWinNginxDlg::OnMenuRuntimeInstall)
	ON_COMMAND(ID_MYSQL_REPWD, &CWinNginxDlg::OnMysqlRepwd)
	ON_COMMAND(ID_MYSQL_COD, &CWinNginxDlg::OnMysqlCmd)
	ON_COMMAND(ID_MENU_SYSSET, &CWinNginxDlg::OnMenuSysSet)
	ON_COMMAND(ID_MENU_PHPCONFIG, &CWinNginxDlg::OnMenuPhpConfig)
	ON_COMMAND(ID_MENU_NGINXCONFIG, &CWinNginxDlg::OnMenuNginxConfig)
END_MESSAGE_MAP()


BOOL CWinNginxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	// Other Init Code
	Init();
	return TRUE; 
}
void CWinNginxDlg::Init(){
	//检测nginx默认配置
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\localhost.conf"))){
		DefWebSite(wng.GetAppPath() + _T("\\wwwroot"), _T("default"), _T("80"), _T("localhost"));
	}
	//检测nginx默认配置
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\php\\php.ini"))){
		DefPhpSet();
	}
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\mysql\\my.ini"))){
		DefMySqlSet();
	}
	//初始显示信息
	CString pv = wng.GetFileVersion(wng.GetAppPath() + _T("\\application\\php\\php.exe"));
	
	vector <CString> nv = wng.getFileContent(wng.GetAppPath() + _T("\\application\\nginx\\version.txt"));
#if _DEBUG
	CString sv = wng.GetFileVersion(wng.GetAppPath() + _T("\\winginxD.exe"));
#else
	CString sv = wng.GetFileVersion(wng.GetAppPath() + _T("\\winginx.exe"));
#endif
	
	CString mv = wng.GetFileVersion(wng.GetAppPath() + _T("\\application\\mysql\\bin\\mysql.exe"));

	if (wng.IsFileExist(wng.GetAppPath() + _T("\\application\\php\\php.exe")))
	{
		vector <CString> pvi;
		pvi = wng.SplitCString(pv, _T("."));

		if (wng.IsFileExist(wng.GetAppPath() + _T("\\webserver\\php\\php" + pvi[0] + _T("ts.dll"))))
		{
			m_info_php.SetWindowText(pv);
		}
		else
		{
			m_info_php.SetWindowText(pv + _T("-nts"));
		}
	}
	else
	{
		m_info_php.SetWindowText(_T("php 目录为空"));
	}
	
	CString os_version, os_info;

	wng.GetOSVersion(os_version, os_info);
	m_info_system.SetWindowText(os_version + _T(" ") + os_info);
	m_info_nginx.SetWindowText(nv[0]);
	m_version.SetWindowText(sv);
	m_info_mysql.SetWindowText(mv);
	//按钮初始化
	m_btn_stop.EnableWindow(FALSE);
	
	vector<CString> pid = wng.getProcessAll();
	if (count(pid.begin(), pid.end(), _T("php-cgi.exe")) != 0)
	{
		serverState[0] = 1;
		m_state_php.SetWindowText(_T("[已运行]"));

	}
	else
	{
		serverState[0] = 0;
		m_state_php.SetWindowText(_T("[已停止]"));

	}
	if (count(pid.begin(), pid.end(), _T("nginx.exe")) != 0)
	{
		serverState[1] = 1;
		m_state_nginx.SetWindowText(_T("[已运行]"));

	}
	else
	{
		serverState[1] = 0;
		m_state_nginx.SetWindowText(_T("[已停止]"));

	}
	if (count(pid.begin(), pid.end(), _T("mysqld.exe")) != 0)
	{
		serverState[2] = 1;
		m_state_mysql.SetWindowText(_T("[已运行]"));

	}
	else
	{
		serverState[2] = 0;
		m_state_mysql.SetWindowText(_T("[已停止]"));

	}
	pid.swap(vector<CString>());
	if ((serverState[0] + serverState[0] + serverState[0]) > 1)
	{
		m_btn_start.EnableWindow(FALSE);
		m_btn_stop.EnableWindow(TRUE);
	}
	else
	{
		m_btn_start.EnableWindow(TRUE);
		m_btn_stop.EnableWindow(FALSE);
	}
	SetTimer(1, 2000, NULL);
	//初始化日志
	DWORD dwStyle = m_log.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_log.SetExtendedStyle(dwStyle);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_log.InsertColumn(0, NULL, LVCFMT_LEFT, 210);
	m_log.InsertColumn(1, NULL, LVCFMT_LEFT, 140);
	wng.writeLog(&m_log, _T("欢迎使用winginx"));
	//初始化运行时间
	start_time = GetTickCount();
	SetTimer(2, 1000, NULL);
	//
	switchPhpVersionMenu(ID_PHP_SEVEN_ONE);
}
void CWinNginxDlg::toTray()//最小化到托盘
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, _T("WinNginx"));
	Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标  
	ShowWindow(SW_HIDE);
}

LRESULT CWinNginxDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
	if (wParam != IDR_MAINFRAME) return 1;
	switch (lParam)
	{
	case WM_LBUTTONUP://左键单击显示主界面
	{
		this->ShowWindow(SW_SHOW);
		SetForegroundWindow();
		DeleteTray();
	}break;
	case WM_RBUTTONUP://右击弹出菜单
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MFT_STRING, IDR_SHOW, _T("界面"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_START, _T("启动服务"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_STOP, _T("停止服务"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_RESTART, _T("重启服务"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_EXIT, _T("退出"));
		SetForegroundWindow();
		if (serverState[0] + serverState[1] + serverState[2]>1)
		{
			EnableMenuItem(menu, IDR_SYS_START, MF_GRAYED);
		}
		else
		{
			EnableMenuItem(menu, IDR_SYS_STOP, MF_GRAYED);

		}
		//EnableMenuItem(menu,IDR_OTHER,MF_GRAYED);//"其他"菜单变灰
		//menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x,lpoint->y,this);//显示菜单
		int mc = TrackPopupMenu(menu, TPM_RETURNCMD, lpoint->x, lpoint->y, NULL, this->m_hWnd, NULL);//显示菜单并获取选项ID
		if (mc == IDR_SHOW)
		{
			this->ShowWindow(SW_SHOW);
			SetForegroundWindow();
			DeleteTray();
		}
		else if (mc == IDR_SYS_START){
			OnBtnStart();
		}
		else if (mc = IDR_SYS_STOP){
			OnBtnStop();
		}
		else if (mc = IDR_SYS_RESTART){
			OnBtnRestart();
		}
		else if (mc = IDR_SYS_EXIT){
			PostMessage(WM_QUIT, 0, 0);
			OnBtnStop();
		}
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
	}break;
	case WM_LBUTTONDBLCLK:
	{
		//双击不处理
	}break;
	}
	return 0;
}

void CWinNginxDlg::DeleteTray()//销毁托盘图标
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, _T("WinNginx"));
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CWinNginxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else if (nID == SC_MAXIMIZE) //最大化 
	{
		this->ShowWindow(SW_SHOW);
	}
	else if (nID == SC_MINIMIZE) //最小化，把他隐藏起来 
	{
		this->ShowWindow(SW_HIDE);
		toTray();
	}
	else if (nID == SC_CLOSE)
	{
		UINT i;
		i = MessageBox(_T("关闭程序并退出？选择否最小化到托盘。"), _T("温馨提示"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDNO)
		{
			toTray();
		}
		else
		{
			PostMessage(WM_QUIT, 0, 0);
		}
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CWinNginxDlg::OnPaint()
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

HCURSOR CWinNginxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CWinNginxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATE_PHP:
		if (serverState[0] != 0){
			pDC->SetTextColor(RGB(0, 255, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	break;
	case IDC_STATE_NGINX:
		if (serverState[1] != 0){
			pDC->SetTextColor(RGB(0, 255, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		break;
	case IDC_STATE_MYSQL:
		if (serverState[2] != 0){
			pDC->SetTextColor(RGB(0, 255, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		break;
	}
	return hbr;
}

void CWinNginxDlg::OnTimer(UINT_PTR nIDEvent)
{
	vector<CString> pid;
	CString run_time;
	long n_time;
	long diff;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
	switch (nIDEvent)
	{

	case 1:
		if ((serverState[0] + serverState[0] + serverState[0])>1)
		{
			m_btn_start.EnableWindow(FALSE);
			m_btn_stop.EnableWindow(TRUE);
		}
		else
		{
			m_btn_start.EnableWindow(TRUE);
			m_btn_stop.EnableWindow(FALSE);
		}
		 pid = wng.getProcessAll();
		if (count(pid.begin(), pid.end(), _T("php-cgi.exe")) != 0)
		{
			serverState[0] = 1;
			m_state_php.SetWindowText(_T("[已运行]"));

		}
		else
		{
			serverState[0] = 0;
			m_state_php.SetWindowText(_T("[已停止]"));

		}
		if (count(pid.begin(), pid.end(), _T("nginx.exe")) != 0)
		{
			serverState[1] = 1;
			m_state_nginx.SetWindowText(_T("[已运行]"));

		}
		else
		{
			serverState[1] = 0;
			m_state_nginx.SetWindowText(_T("[已停止]"));

		}
		if (count(pid.begin(), pid.end(), _T("mysqld.exe")) != 0)
		{
			serverState[2] = 1;
			m_state_mysql.SetWindowText(_T("[已运行]"));

		}
		else
		{
			serverState[2] = 0;
			m_state_mysql.SetWindowText(_T("[已停止]"));

		}
		pid.swap(vector<CString>());
		
		break;
	case 2:         
		n_time = GetTickCount();
		diff = n_time - start_time;
		nDay = diff / 86400000;
		nHour = (diff / 3600000)%24;
		nMinute = (diff / 60000)%60;
		nSecond = (diff / 1000)%60;
		run_time.Format(_T("服务器已运行:%d天%d小时%d分%d秒"),nDay,nHour,nMinute,nSecond);
		m_state_server.SetWindowText(run_time);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CWinNginxDlg::switchPhpVersionMenu(int Ids){
	CMenu *pMenu = GetMenu();
	ASSERT_VALID(pMenu);
	CMenu *pSubMenu = pMenu->GetSubMenu(1);
	ASSERT_VALID(pSubMenu);
	pSubMenu->CheckMenuItem(Ids, MF_BYCOMMAND | MF_CHECKED);
	pSubMenu->EnableMenuItem(Ids, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	if (php_version)
	{
		pSubMenu->EnableMenuItem(php_version, MF_BYCOMMAND | MF_ENABLED);
		pSubMenu->CheckMenuItem(php_version, MF_BYCOMMAND | MF_UNCHECKED);
	}
	php_version = Ids;
}
void CWinNginxDlg::DefWebSite(CString root, CString name, CString port, CString domain)
{
	CString tempFile;
	CString str;
	tempFile = _T("vhost_http_template.conf");
	CStdioFile myFile, File;
	CString strText;
	if (myFile.Open(wng.GetAppPath() + _T("\\application\\nginx\\conf\\" + tempFile), CFile::modeRead))
	{
		//读取
		while (myFile.ReadString(str))
		{
			strText += str + _T("\r\n");
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
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")))){
		File.Open(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")), CFile::modeCreate | CFile::modeReadWrite);
		File.WriteString(strText);
		if (wng.IsFileExist(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")))){
			wng.setConfig(_T("网站配置列表"), name, name + _T("|") + domain + _T("|") + port + _T("|") + root, wng.GetAppPath() + _T("\\\\application\\webList.ini"));
		}
		strText.ReleaseBuffer();
		File.Close();
	}
}
void CWinNginxDlg::DefMySqlSet()
{
	BOOL m = CopyFile(wng.GetAppPath() + _T("\\application\\def_config\\my.ini"), wng.GetAppPath() + _T("\\application\\mysql\\my.ini"), TRUE);
	if (m)
	{
		wng.setConfig(_T("mysqld"), _T("basedir"), wng.GetAppPath() + _T("\\application\\mysql"), wng.GetAppPath() + _T("\\application\\mysql\\my.ini"));
		wng.setConfig(_T("mysqld"), _T("datadir"), wng.GetAppPath() + _T("\\application\\mysql\\data"), wng.GetAppPath() + _T("\\application\\mysql\\my.ini"));
	}
	else
	{
		AfxMessageBox(_T("初始化PHP配置文件错误"));
	}
}
void CWinNginxDlg::DefPhpSet()
{
	BOOL m = CopyFile(wng.GetAppPath() + _T("\\application\\def_config\\php.ini"), wng.GetAppPath() + _T("\\application\\php\\php.ini"), TRUE);
	if (m)
	{
		wng.setConfig(_T("PHP"), _T("extension_dir"), _T("\"") + wng.GetAppPath() + _T("\\application\\php\\ext") + _T("\""), wng.GetAppPath() + _T("\\application\\php\\php.ini"));
	}
	else
	{
		AfxMessageBox(_T("初始化PHP配置文件错误"));
	}
}
void CWinNginxDlg::OnBtnStart()
{

	ShellExecute(NULL, NULL, wng.GetAppPath() + _T("\\application\\start.bat"), NULL, NULL, SW_HIDE);
	wng.writeLog(&m_log,_T("启动nginx..."));
	wng.writeLog(&m_log, _T("启动php..."));
	wng.writeLog(&m_log, _T("启动mysql..."));
}

void CWinNginxDlg::OnBtnStop()
{
	ShellExecute(NULL, NULL, wng.GetAppPath() + _T("\\application\\stop.bat"), NULL, NULL, SW_HIDE);
	wng.writeLog(&m_log, _T("关闭nginx..."));
	wng.writeLog(&m_log, _T("关闭php..."));
	wng.writeLog(&m_log, _T("关闭mysql..."));
}


void CWinNginxDlg::OnBtnRestart()
{
	OnBtnStop();
	OnBtnStart();
}


void CWinNginxDlg::OnMenuAboutCompright()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CWinNginxDlg::OnBtnWebManage()
{
	CWnweblistDlg dlg;
	dlg.DoModal();
	
}


void CWinNginxDlg::OndBtnAddWeb()
{
	CWNaddwebDlg dlg;
	dlg.DoModal();
}


void CWinNginxDlg::OnBtnCheckRuntime()
{
	wng.writeLog(&m_log, _T("Visual C++ Redistributable checking..."));
	//2005 sp1 x86
	if (MsiQueryProductState(_T("{A49F249F-0C91-497F-86DF-B2585E8E76B7}")) != -1){
		wng.writeState(&m_log, _T("VC 2005 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2005 Redistributable"), _T("No Install"));
	}
	//2005 x64
	if (MsiQueryProductState(_T("{6E8E85E8-CE4B-4FF5-91F7-04999C9FAE6A}")) != -1){
		wng.writeState(&m_log, _T("VC 2005 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2005 x64 Redistributable"), _T("No Install"));

	}
	//2008 x86
	if (MsiQueryProductState(_T("{FF66E9F6-83E7-3A3E-AF14-8DE9A809A6A4}")) != -1){
		wng.writeState(&m_log, _T("VC 2008 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2008 Redistributable"), _T("No Install"));

	}
	//2008 x64
	if (MsiQueryProductState(_T("{350AA351-21FA-3270-8B7A-835434E766AD}")) != -1){
		wng.writeState(&m_log, _T("VC 2008 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2008 x64 Redistributable"), _T("No Install"));

	}
	//2008 sp1 x86
	if (MsiQueryProductState(_T("{9A25302D-30C0-39D9-BD6F-21E6EC160475}")) != -1){
		wng.writeState(&m_log, _T("VC 2008 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2008 Redistributable"), _T("No Install"));

	}
	//2008 sp1 x64
	if (MsiQueryProductState(_T("{8220EEFE-38CD-377E-8595-13398D740ACE}")) != -1){
		wng.writeState(&m_log, _T("VC 2008 sp1 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2008 sp1 x64 Redistributable"), _T("No Install"));

	}
	//2012 x64
	if (MsiQueryProductState(_T("{764384C5-BCA9-307C-9AAC-FD443662686A}")) != -1){
		wng.writeState(&m_log, _T("VC 2012 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2012 x64 Redistributable"), _T("No Install"));

	}
	//2012 x86 vc11
	if (MsiQueryProductState(_T("{764384C5-BCA9-307C-9AAC-FD443662686A}")) != -1){
		wng.writeState(&m_log, _T("VC 2012 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2012 x64 Redistributable"), _T("No Install"));

	}
	//2012 x64 vc11
	if (MsiQueryProductState(_T("{B175520C-86A2-35A7-8619-86DC379688B9}")) != -1){
		wng.writeState(&m_log, _T("VC 2012 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2012 x64 Redistributable"), _T("No Install"));

	}
	//2015 x86 vc14
	if (MsiQueryProductState(_T("{8c3f057e-d6a6-4338-ac6a-f1c795a6577b}")) != -1){
		wng.writeState(&m_log, _T("VC 2015 x86 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2015 x86 Redistributable"), _T("No Install"));

	}
	//2015 x64 vc14
	if (MsiQueryProductState(_T("{7b178cda-9740-4701-a92a-f168d213b343}")) != -1){
		wng.writeState(&m_log, _T("VC 2015 x64 Redistributable"), _T("Installed"));
	}
	else
	{
		wng.writeState(&m_log, _T("VC 2015 x64 Redistributable"), _T("No Install"));

	}
	wng.writeLog(&m_log, _T("VC Redistributable checked."));
}
void CWinNginxDlg::switchPhpVersion(int version,int type){
	switch (version)
	{
	case PHP_VSRSION_FIVETWO:
		if (type == 1001){
			switchPhp(_T("5.2"), version);
		}
		if (type == 1002){
			switchPhp(_T("5.2-nts"), version);
		}
		if (type == 1003){
			switchPhp(_T("5.2_x64"), version);
		}
		if (type == 1004){
			switchPhp(_T("5.2-nts_x64"), version);
		}
		break;
	case PHP_VSRSION_FIVESEX:
		if (type == 1001){
			switchPhp(_T("5.6"), version);
		}
		if (type == 1002){
			switchPhp(_T("5.6-nts"), version);
		}
		if (type == 1003){
			switchPhp(_T("5.6_x64"), version);
		}
		if (type == 1004){
			switchPhp(_T("5.6-nts_x64"), version);
		}
		break;
	case PHP_VSRSION_SEVEN:
		if (type == 1001){
			switchPhp(_T("7.0"), version);
		}
		if (type == 1002){
			switchPhp(_T("7.0-nts"), version);
		}
		if (type == 1003){
			switchPhp(_T("7.0_x64"), version);
		}
		if (type == 1004){
			switchPhp(_T("7.0-nts_x64"), version);
		}
		break;
	case PHP_VSRSION_SEVENONE:
		if (type == 1001){
			switchPhp(_T("7.1"), version);
		}
		if (type == 1002){
			switchPhp(_T("7.1-nts"), version);
		}
		if (type == 1003){
			switchPhp(_T("7.1_x64"), version);
		}
		if (type == 1004){
			switchPhp(_T("7.1-nts_x64"), version);
		}
		break;
	case PHP_VSRSION_SEVENTWO:
		if (type == 1001){
			switchPhp(_T("7.2"), version);
		}
		if (type == 1002){
			switchPhp(_T("7.2-nts"), version);
		}
		if (type == 1003){
			switchPhp(_T("7.2_x64"), version);
		}
		if (type == 1004){
			switchPhp(_T("7.2-nts_x64"), version);
		}
		break;
	case PHP_VSRSION_SEVENTHREE:
		if (type == 1001){
			switchPhp(_T("7.3"), version);
		}
		if (type == 1002){
			switchPhp(_T("7.3-nts"), version);
		}
		if (type == 1003){
			switchPhp(_T("7.3_x64"), version);
		}
		if (type == 1004){
			switchPhp(_T("7.3-nts_x64"), version);
		}
		break;
	default:
		return;
		break;
	}
}
void CWinNginxDlg::switchPhp(CString v, int Ids){
	
	wng.writeLog(&m_log, _T("php版本切换中..."));
	CopyFile(wng.GetAppPath() + _T("\\application\\php\\php.ini"), wng.GetAppPath() + _T("\\application\\php_ext\\php_back.ini"), TRUE);
	m_strPath = wng.GetAppPath() + _T("\\application\\php_ext\\php-") + v + _T("-nts_x86.7z");
	m_desPath = wng.GetAppPath() + _T("\\application\\php");
	wng.deleteDirectory(m_desPath);
	wng.writeLog(&m_log, _T("清除旧版本..."));
	if (!wng.IsFileExist(m_strPath))
	{
		wng.writeLog(&m_log, _T("php版本不存在..."));
		UINT i;
		i = MessageBox(_T("php版本不存在，是否安装？"), _T("温馨提示"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDNO)
		{
			return;
		}
		else
		{
			//安装php
		}
		return;
	}
	else
	{
// 		char* filepath;
// 		char* despath;
// 		USES_CONVERSION;
// 		filepath = T2A(m_strPath);
// 		despath = T2A(m_desPath);
// 		string sp = filepath;
// 		string dp = despath;
// 
// 		string srcFilePath = "D:\\client\\winginx\\winginx\\application\\php_ext\\php-5.2.zip";
// 		string tempdir = "D:\\www";
//      wng.UnzipFile(sp, tempdir)
		wng.writeLog(&m_log, _T("解压文件..."));
		if (wng.Un7zZip(m_strPath, m_desPath, 0)==14){
			switchPhpVersionMenu(Ids);
			wng.writeLog(&m_log, _T("php版本已切换至") + v + _T("..."));
			wng.writeLog(&m_log, _T("重启服务器..."));
			OnBtnRestart();
		}
		else
		{
			wng.writeLog(&m_log, _T("php版本切换失败..."));
		}
	}
}
void CWinNginxDlg::OnBtnServerback()
{
	CWNbackDlg dlg;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuFiveTwo()
{
	switchPhpVersion(52,1001);
}


void CWinNginxDlg::OnMenuFiveSix()
{

	switchPhpVersion(56, 1001);
	
}


void CWinNginxDlg::OnMenuSeven()
{
	switchPhpVersion(70, 1001);

}


void CWinNginxDlg::OnMenuSevenOne()
{
	switchPhpVersion(71, 1001);
}


void CWinNginxDlg::OnMenuSevenTwo()
{
	switchPhpVersion(72, 1001);
}


void CWinNginxDlg::OnMenuSevenThree()
{
	switchPhpVersion(73, 1001);
}


void CWinNginxDlg::OnMenuRuntimeInstall()
{
	WNruntimeDlg dlg;
	dlg.DoModal();
}


void CWinNginxDlg::OnMysqlRepwd()
{
	ShellExecute(NULL, NULL, wng.GetAppPath() + _T("\\application\\mysql\\bin\\reset.bat"), NULL, NULL, SW_HIDE);
	wng.writeLog(&m_log, _T("root密码已重置为root"));
}


void CWinNginxDlg::OnMysqlCmd()
{
	//ShellExecute(NULL, NULL, wng.GetAppPath() + _T("\\application\\mysql\\bin\\cmd.bat"), NULL, NULL, SW_HIDE);
	CString dir = _T("/k cd ") +wng.GetAppPath()+ _T("/application/mysql/bin");
	wng.ShellRun(_T("cmd.exe"), dir, SW_SHOW);
}


void CWinNginxDlg::OnMenuSysSet()
{
	AfxMessageBox(_T("咦，似乎还没想到要配置啥..."));
}


void CWinNginxDlg::OnMenuPhpConfig()
{
	CString dir = wng.GetAppPath() + _T("\\application\\php\\php.ini");
	wng.ShellRun(_T("notepad.exe "), dir, SW_SHOW);
}


void CWinNginxDlg::OnMenuNginxConfig()
{
	CString dir = wng.GetAppPath() + _T("\\application\\nginx\\conf\\nginx.conf");
	wng.ShellRun(_T("notepad.exe "), dir, SW_SHOW);
}
