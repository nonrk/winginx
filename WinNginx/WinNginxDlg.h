
// WinNginxDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWinNginxDlg 对话框
class CWinNginxDlg : public CDialogEx
{
// 构造
public:
	CWinNginxDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WINNGINX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_info_system;
	CStatic m_info_php;
	CStatic m_info_nginx;
	CStatic m_info_mysql;
	CStatic m_version;
	CStatic m_state_php;
	CStatic m_state_nginx;
	CStatic m_state_mysql;
	CStatic m_state_server;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_log;
	CButton m_btn_start;
	CButton m_btn_stop;
	CButton m_btn_restart;
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnRestart();
	void Init();
	void toTray();
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	void DeleteTray();
	CButton m_btn_add;
	CButton m_btn_web;
	CButton m_btn_runtime;
	CButton m_btn_server_back;
	CString m_strPath;
	CString m_desPath;
	afx_msg void OnMenuAboutCompright();
	afx_msg void OnBtnWebManage();
	afx_msg void OndBtnAddWeb();
	afx_msg void OnBtnCheckRuntime();
	afx_msg void OnBtnServerback();
	void switchPhpVersionMenu(int Ids);
	afx_msg void OnMenuFiveSix();
	afx_msg void OnMenuSeven();
	afx_msg void OnMenuSevenOne();
	afx_msg void OnMenuSevenTwo();
	afx_msg void OnMenuSevenThree();
	afx_msg void OnMenuFiveTwo();
	afx_msg void OnMenuRuntimeInstall();
	afx_msg void OnMysqlRepwd();
	afx_msg void OnMysqlCmd();
	afx_msg void OnMenuSysSet();
	afx_msg void OnMenuPhpConfig();
	afx_msg void OnMenuNginxConfig();
	void DefWebSite(CString root, CString name, CString port, CString domain);
	void DefMySqlSet();
	void DefPhpSet();
	void switchPhpVersion(int version, int type);
	void switchPhp(CString v, int Ids);
};
