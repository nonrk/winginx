#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;
// CWnweblistDlg �Ի���

class CWnweblistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWnweblistDlg)

public:
	CWnweblistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWnweblistDlg();

// �Ի�������
	enum { IDD = IDD_DLG_WEB_LIST };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	afx_msg void OnNMDblclkWeb(NMHDR *pNMHDR, LRESULT *pResult);
	void Init();
	vector<CString> getWebList();
};
