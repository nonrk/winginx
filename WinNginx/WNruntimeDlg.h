#pragma once
#include "afxwin.h"


// WNruntimeDlg �Ի���

class WNruntimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WNruntimeDlg)

public:
	WNruntimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WNruntimeDlg();

// �Ի�������
	enum { IDD = IDD_DLG_INSTALL_RUNTIME };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void Init();
	CComboBox m_list;
	afx_msg void OnBtnStartInstall();
};
