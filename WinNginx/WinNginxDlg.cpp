
#include "stdafx.h"
#include "afxdialogex.h"
#include "windows.h"
#include "winbase.h"
#include <shellapi.h>
#include <msi.h>
#include "WinNginx.h"
#include "WinNginxDlg.h"
#include "WNaddwebDlg.h"
#include "WNGlobal.h"
#include "WnweblistDlg.h"
#include "WNbackDlg.h"
#include "WNswitchDlg.h"
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
	//���nginxĬ������
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\localhost.conf"))){
		DefWebSite(wng.GetAppPath() + _T("\\wwwroot"), _T("default"), _T("80"), _T("localhost"));
	}
	//���nginxĬ������
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\php\\php.ini"))){
		DefPhpSet();
	}
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\mysql\\my.ini"))){
		DefMySqlSet();
	}
	//��ʼ��ʾ��Ϣ
	CString pv = wng.GetFileVersion(wng.GetAppPath() + _T("\\application\\php\\php.exe"));
	vector <CString> nv = wng.getFileContent(wng.GetAppPath() + _T("\\application\\nginx\\version.txt"));
#if _DEBUG
	CString sv = wng.GetFileVersion(wng.GetAppPath() + _T("\\winginxD.exe"));
#else
	CString sv = wng.GetFileVersion(wng.GetAppPath() + _T("\\winginx.exe"));
#endif
	
	CString mv = wng.GetFileVersion(wng.GetAppPath() + _T("\\application\\mysql\\bin\\mysql.exe"));
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

	CString os_version, os_info;

	wng.GetOSVersion(os_version, os_info);
	m_info_system.SetWindowText(os_version + _T(" ") + os_info);
	m_info_nginx.SetWindowText(nv[0]);
	m_version.SetWindowText(sv);
	m_info_mysql.SetWindowText(mv);
	//��ť��ʼ��
	m_btn_stop.EnableWindow(FALSE);
	
	vector<CString> pid = wng.getProcessAll();
	if (count(pid.begin(), pid.end(), _T("php-cgi.exe")) != 0)
	{
		serverState[0] = 1;
		m_state_php.SetWindowText(_T("[������]"));

	}
	else
	{
		serverState[0] = 0;
		m_state_php.SetWindowText(_T("[��ֹͣ]"));

	}
	if (count(pid.begin(), pid.end(), _T("nginx.exe")) != 0)
	{
		serverState[1] = 1;
		m_state_nginx.SetWindowText(_T("[������]"));

	}
	else
	{
		serverState[1] = 0;
		m_state_nginx.SetWindowText(_T("[��ֹͣ]"));

	}
	if (count(pid.begin(), pid.end(), _T("mysqld.exe")) != 0)
	{
		serverState[2] = 1;
		m_state_mysql.SetWindowText(_T("[������]"));

	}
	else
	{
		serverState[2] = 0;
		m_state_mysql.SetWindowText(_T("[��ֹͣ]"));

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
	//��ʼ����־
	DWORD dwStyle = m_log.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_log.SetExtendedStyle(dwStyle);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_log.InsertColumn(0, NULL, LVCFMT_LEFT, 210);
	m_log.InsertColumn(1, NULL, LVCFMT_LEFT, 140);
	wng.writeLog(&m_log, _T("��ӭʹ��winginx"));
	//��ʼ������ʱ��
	start_time = GetTickCount();
	SetTimer(2, 1000, NULL);
	//
	switchPhpVersion(ID_PHP_SEVEN_ONE);
}
void CWinNginxDlg::toTray()//��С��������
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, _T("WinNginx"));
	Shell_NotifyIcon(NIM_ADD, &nid);//�����������ͼ��  
	ShowWindow(SW_HIDE);
}

LRESULT CWinNginxDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
	if (wParam != IDR_MAINFRAME) return 1;
	switch (lParam)
	{
	case WM_LBUTTONUP://���������ʾ������
	{
		this->ShowWindow(SW_SHOW);
		SetForegroundWindow();
		DeleteTray();
	}break;
	case WM_RBUTTONUP://�һ������˵�
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ��
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MFT_STRING, IDR_SHOW, _T("����"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_START, _T("��������"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_STOP, _T("ֹͣ����"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_RESTART, _T("��������"));
		menu.AppendMenu(MFT_STRING, IDR_SYS_EXIT, _T("�˳�"));
		SetForegroundWindow();
		if (serverState[0] + serverState[1] + serverState[2]>1)
		{
			EnableMenuItem(menu, IDR_SYS_START, MF_GRAYED);
		}
		else
		{
			EnableMenuItem(menu, IDR_SYS_STOP, MF_GRAYED);

		}
		//EnableMenuItem(menu,IDR_OTHER,MF_GRAYED);//"����"�˵����
		//menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x,lpoint->y,this);//��ʾ�˵�
		int mc = TrackPopupMenu(menu, TPM_RETURNCMD, lpoint->x, lpoint->y, NULL, this->m_hWnd, NULL);//��ʾ�˵�����ȡѡ��ID
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
			OnBtnStop();
			PostMessage(WM_QUIT, 0, 0);
		}
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
	}break;
	case WM_LBUTTONDBLCLK:
	{
		//˫��������
	}break;
	}
	return 0;
}

void CWinNginxDlg::DeleteTray()//��������ͼ��
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
	}else if (nID == SC_MAXIMIZE) //��� 
	{
		this->ShowWindow(SW_SHOW);
	}
	else if (nID == SC_MINIMIZE) //��С���������������� 
	{
		this->ShowWindow(SW_HIDE);
		toTray();
	}
	else if (nID == SC_CLOSE)
	{
		UINT i;
		i = MessageBox(_T("�رճ����˳���ѡ�����С�������̡�"), _T("��ܰ��ʾ"), MB_YESNO | MB_ICONQUESTION);
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
			m_state_php.SetWindowText(_T("[������]"));

		}
		else
		{
			serverState[0] = 0;
			m_state_php.SetWindowText(_T("[��ֹͣ]"));

		}
		if (count(pid.begin(), pid.end(), _T("nginx.exe")) != 0)
		{
			serverState[1] = 1;
			m_state_nginx.SetWindowText(_T("[������]"));

		}
		else
		{
			serverState[1] = 0;
			m_state_nginx.SetWindowText(_T("[��ֹͣ]"));

		}
		if (count(pid.begin(), pid.end(), _T("mysqld.exe")) != 0)
		{
			serverState[2] = 1;
			m_state_mysql.SetWindowText(_T("[������]"));

		}
		else
		{
			serverState[2] = 0;
			m_state_mysql.SetWindowText(_T("[��ֹͣ]"));

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
		run_time.Format(_T("������������:%d��%dСʱ%d��%d��"),nDay,nHour,nMinute,nSecond);
		m_state_server.SetWindowText(run_time);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CWinNginxDlg::switchPhpVersion(int Ids){
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
		//��ȡ
		while (myFile.ReadString(str))
		{
			strText += str + _T("\r\n");
		}
		myFile.Close();
	}
	else
	{
		AfxMessageBox(_T("ģ���ļ�������"));
	}

	//�滻�ؼ���
	strText.Replace(_T("SERVER_DOMAIN"), domain);
	strText.Replace(_T("SERVER_PORT"), port);
	strText.Replace(_T("SERVER_ROOT"), root);
	//����
	//File.SeekToEnd();
	if (!wng.IsFileExist(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")))){
		File.Open(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")), CFile::modeCreate | CFile::modeReadWrite);
		File.WriteString(strText);
		if (wng.IsFileExist(wng.GetAppPath() + _T("\\application\\nginx\\conf\\vhost\\" + domain + _T(".conf")))){
			wng.setConfig(_T("��վ�����б�"), name, name + _T("|") + domain + _T("|") + port + _T("|") + root, wng.GetAppPath() + _T("\\\\application\\webList.ini"));
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
		AfxMessageBox(_T("��ʼ��PHP�����ļ�����"));
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
		AfxMessageBox(_T("��ʼ��PHP�����ļ�����"));
	}
}
void CWinNginxDlg::OnBtnStart()
{

	ShellExecute(NULL, NULL, wng.GetAppPath() + _T("\\application\\start.bat"), NULL, NULL, SW_HIDE);
	wng.writeLog(&m_log,_T("����nginx..."));
	wng.writeLog(&m_log, _T("����php..."));
	wng.writeLog(&m_log, _T("����mysql..."));
}

void CWinNginxDlg::OnBtnStop()
{
	ShellExecute(NULL, NULL, wng.GetAppPath() + _T("\\application\\stop.bat"), NULL, NULL, SW_HIDE);
	wng.writeLog(&m_log, _T("�ر�nginx..."));
	wng.writeLog(&m_log, _T("�ر�php..."));
	wng.writeLog(&m_log, _T("�ر�mysql..."));
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
void CWinNginxDlg::OnBtnServerback()
{
	CWNbackDlg dlg;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuFiveTwo()
{
	//�жϰ汾�Ƿ����
	switchPhpVersion(ID_PHP_FIVETWO);
	//�����ļ�����
	CWNswitchDlg dlg;
	dlg.version = 52;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuFiveSix()
{
	//�жϰ汾�Ƿ����
	switchPhpVersion(ID_PHP_FIVESIX);
	CWNswitchDlg dlg;
	dlg.version = 56;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuSeven()
{
	switchPhpVersion(ID_PHP_SEVEN);
	CWNswitchDlg dlg;
	dlg.version = 70;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuSevenOne()
{
	switchPhpVersion(ID_PHP_SEVEN_ONE);
	CWNswitchDlg dlg;
	dlg.version = 71;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuSevenTwo()
{
	switchPhpVersion(ID_PHP_SEVENTWO);
	CWNswitchDlg dlg;
	dlg.version = 72;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuSevenThree()
{
	switchPhpVersion(ID_PHP_SEVENTHREE);
	CWNswitchDlg dlg;
	dlg.version = 73;
	dlg.DoModal();
}


void CWinNginxDlg::OnMenuRuntimeInstall()
{
	// TODO:  �ڴ���������������
}


void CWinNginxDlg::OnMysqlRepwd()
{
	// TODO:  �ڴ���������������
}


void CWinNginxDlg::OnMysqlCmd()
{
	// TODO:  �ڴ���������������
}


void CWinNginxDlg::OnMenuSysSet()
{
	// TODO:  �ڴ���������������
}


void CWinNginxDlg::OnMenuPhpConfig()
{
	// TODO:  �ڴ���������������
}


void CWinNginxDlg::OnMenuNginxConfig()
{
	// TODO:  �ڴ���������������
}
