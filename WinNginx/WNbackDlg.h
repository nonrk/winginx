#pragma once
#include "afxwin.h"


// CWNbackDlg 对话框

class CWNbackDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWNbackDlg)

public:
	CWNbackDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWNbackDlg();

// 对话框数据
	enum { IDD = IDD_DLG_BACK };

protected:
	virtual BOOL CWNbackDlg::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_dir;
	afx_msg void OnBtnStartBack();
	afx_msg void OnBtnRestore();
	CButton m_btn_restore;
	CButton m_btn_start;
	CButton m_ck_config;
	CButton m_ck_data;
	CButton m_ck_site;
	CButton m_ck_all;
	void Init();
	afx_msg void OnBtnDir();
	afx_msg void OnCkAll();
	void checkCheckBox();
	afx_msg void OnCkConfig();
	afx_msg void OnCkData();
	afx_msg void OnCkSite();
};
