#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWNswitchDlg �Ի���

class CWNswitchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWNswitchDlg)

public:
	CWNswitchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWNswitchDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SWITCH };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_status;
	CStatic m_version;
	int version;
	CString m_strPath;
	CString m_desPath;
	CProgressCtrl m_progress;
	void Init();
	afx_msg void OnDestroy();
};
