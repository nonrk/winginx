#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;
// CWnweblistDlg 对话框

class CWnweblistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWnweblistDlg)

public:
	CWnweblistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWnweblistDlg();

// 对话框数据
	enum { IDD = IDD_DLG_WEB_LIST };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	afx_msg void OnNMDblclkWeb(NMHDR *pNMHDR, LRESULT *pResult);
	void Init();
	vector<CString> getWebList();
};
