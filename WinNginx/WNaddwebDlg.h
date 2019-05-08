#pragma once
#include "afxwin.h"


// CWNaddwebDlg 对话框
class CWinNginxDlg;
class CWNaddwebDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWNaddwebDlg)

public:
	CWNaddwebDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWNaddwebDlg();
// 对话框数据
	enum { IDD = IDD_DLG_ADD };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString g_name;
	CString g_domain;
	CString g_port;
	CString g_root;
	CEdit m_domain;
	CEdit m_root;
	afx_msg void OnBtnOpenDir();
	CEdit m_port;
	CButton m_check_https;
	CButton m_btn_save;
	CButton m_btn_add;
	afx_msg void OnBtnSave();
	void Init();
	CEdit m_name;
	void AddWebSite(CString root, CString name, CString port, CString domain, int https);
};
