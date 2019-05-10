#pragma once
#include "afxwin.h"


// WNruntimeDlg 对话框

class WNruntimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WNruntimeDlg)

public:
	WNruntimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WNruntimeDlg();

// 对话框数据
	enum { IDD = IDD_DLG_INSTALL_RUNTIME };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Init();
	CComboBox m_list;
	afx_msg void OnBtnStartInstall();
};
